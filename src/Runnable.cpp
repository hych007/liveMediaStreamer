/*
 *  Runnable.cpp - This is the interface between Workers and Filters
 *  Copyright (C) 2014  Fundació i2CAT, Internet i Innovació digital a Catalunya
 *
 *  This file is part of liveMediaStreamer.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors:  David Cassany <david.cassany@i2cat.net>
 *
 *
 */

#include <thread>

#include "Runnable.hh"


Runnable::Runnable(bool periodic_) : run(false), periodic(periodic_), id(-1)
{
}

Runnable::~Runnable()
{
}

bool Runnable::ready()
{
    return time < std::chrono::high_resolution_clock::now();
}

void Runnable::sleepUntilReady()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::microseconds teaTime;

    if (!ready()){
        teaTime = std::chrono::duration_cast<std::chrono::microseconds>(time - now);

        std::this_thread::sleep_for(teaTime);
    }
}

std::vector<int> Runnable::runProcessFrame()
{   
    int ret = 0;
    std::vector<int> enabledJobs;
    enabledJobs = processFrame(ret);
    
    time = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(ret);
    
    return enabledJobs;
}

bool Runnable::setId(int id_){
    if (id_ < 0){
        utils::errorMsg("invalid filter Id, only positive values are allowed");
        return false;
    }
    
    if (id >= 0){
        utils::errorMsg("You cannot re-assign the filter Id");
        return false;
    }
    
    id = id_;
    
    return true;
}

void Runnable::setRunning()
{
    run = true;
}

void Runnable::unsetRunning()
{    
    run = false;
}

bool Runnable::isRunning() 
{
    return run;
}

