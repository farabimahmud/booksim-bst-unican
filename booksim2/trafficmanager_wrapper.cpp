// $Id$

/*
 Copyright (c) 2014-2020, Trustess of The University of Cantabria
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this 
 list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "trafficmanager_wrapper.hpp"

namespace Booksim
{

    TrafficManagerWrapper::TrafficManagerWrapper(const Configuration &config,
                                                 const vector<Network *> & net
                                                 )
        : TrafficManager(config, net)
    {
        _ejection_queue.resize(_classes);
        for (int cl=0; cl < _classes; cl++)
        {
            _ejection_queue[cl].resize(_nodes);
        }

        // Stats printing
        _last_print = 0;
        _sample_period = config.GetInt( "sample_period" );
        _sim_state = running;

        // Retrieve and check the number of injection queues
        _injection_queues = config.GetInt("injection_queues");
    }


    TrafficManagerWrapper::~TrafficManagerWrapper()
    {

    }


    // Interface to internal methods of TrafficManager
    long
    TrafficManagerWrapper::GeneratePacket(int source,
                                          int dest,
                                          int size,
                                          int cl,
                                          long time)
    {
        return _GeneratePacket(source, dest, size, cl, GetSimTime() - time);
    }


    // XXX: I think it is not necessary
    bool
    TrafficManagerWrapper::CheckEjectionQueue()
    {
		return true;
    }

    // TODO: override _RetirePacket method of TrafficManager to store flits in a
    // finite queue?
    // _retired_packets holds packets to consume, however it is managed by
    // TrafficManager in _Step, so we cannot use it without overriding _Step().
    // The problem is that it is a huuuuge function.
    // I think the best option is to create a new queue managed and override
    // _RetirePacket. This new queue is going to be managed by
    // TrafficManagerWrapper. I cannot control the number of elements inserted so
    // instead of set the size of the queue I'm going to print a warning message
    // when exceeding the defined size.
    void
    TrafficManagerWrapper::_RetirePacket(Flit * head, Flit * tail) 
    {
        TrafficManager::_RetirePacket(head, tail);
        assert(head);
        assert(tail);
        assert(head->pid == tail->pid);
        assert(head->cl == tail->cl);
        
        //_ejection_queue[head->cl][head->dest].push(head->pid);
        
        // Create retired packet
        //RetiredPacket rp = {head->pid, 
        //                                      head->cl,
        //                                      head->packet_size,
        //                                      tail->atime-head->ctime,
        //                                      tail->atime-head->itime,
        //                                      head->hops,
        //                                      head->hpc.size(),
        //                                      0};
        _ejection_queue[head->cl][head->dest].push(make_pair(*head, *tail));
    }

    // FIXME: check type of returning data
    // This function must be called until it returns -1;
    // XXX: I belive that the best way is to declare a hash table in the caller to
    // map the PID (returning value) with the message type used in it.
    // E.g., in gem5 MsgPtr (or Message) so that way external info to BookSim can be
    // save with ease.
    std::pair<Flit, Flit>
    TrafficManagerWrapper::RetirePacket()
    {
        //std::cout << GetSimTime() << " TrafficManagerWrapper:RetirePacket" << std::endl;
        // XXX: in this implementation class 0 has priority
        for (int cl=0; cl < _classes; cl++){
            for (int dst=0; dst < _nodes; dst++)
            {
                if(!_ejection_queue[cl][dst].empty()) {
                    //Flit * head = _ejection_queue[cl][dst].front();
                    pair<Flit,Flit> rp = _ejection_queue[cl][dst].front();
                    //std::cout << GetSimTime() << " TrafficManagerWrapper:RetirePacket | Front pid: " << pid << std::endl;
                    _ejection_queue[cl][dst].pop();
                    
                    //Flit head = rp.first;
                    //Flit tail = rp.second;

                    return rp;
                }
            }
        }
        Flit head = Flit();
        head.pid = -1;
        return make_pair(head, head);
    }


    void
    TrafficManagerWrapper::RunCycles(int cycles)
    {
        for (int i = 0; i < cycles; i++)
        {
            _Step();
        }
        if(_print_csv_results && _sample_period < GetSimTime() - _last_print)
        {
            std::cout << " Cur PID: " << _cur_pid << std::endl;
            UpdateStats();
            _UpdateOverallStats();
            //DisplayStats();
            DisplayOverallStats();
            _ClearStats();

#ifdef TRACK_FLOWS
            for(int subnet = 0; subnet < _subnets; ++subnet) {
                for(int router = 0; router < _routers; ++router) {
                    Router * const r = _router[subnet][router];
                    for(int c = 0; c < _classes; ++c) {
                        r->ResetFlowStats(c);
                    }
                }
            }
#endif // TRACK_FLOWS

            _last_print = GetSimTime();
        }
    }


    bool
    TrafficManagerWrapper::CheckInFlightPackets()
    {
        bool in_flight_packets = false;
        for (int cl=0; cl < _classes; cl++)
        {
            in_flight_packets |= !_total_in_flight_flits[cl].empty();
            
            if (in_flight_packets) {
                break;
            }
        
            for (int dst=0; dst < _nodes; dst++)
            {
                in_flight_packets |= !_ejection_queue[cl][dst].empty();
                if (in_flight_packets) {
                    break;
                }
            }
            
        }

        return in_flight_packets;
    }

    void
    TrafficManagerWrapper::ClearStats()
    {
        TrafficManager::_ClearStats();
    }

    void
    TrafficManagerWrapper::UpdateSimTime(int cycles)
    {
        _time += cycles;
        assert(_time);
    }
    
    int
    TrafficManagerWrapper::CheckInjectionQueue(int source, int cl)
    {
        int cur_occupancy;
        //BSMOD: Evaluation of the number of injection queues is added
        if(_injection_queues == 1) {
            cur_occupancy = _partial_packets[0][source].size();
        } else {
            cur_occupancy = _partial_packets[cl][source].size();
        }
        return _inj_size - cur_occupancy;
    }
} // namespace Booksim
