#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono>
#include <thread>

/* Implementation of class "MessageQueue" */
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    // create a lock
    std::unique_lock<std::mutex> lock(_mutex);
    // wait for the new message
    _cond.wait(lock, [this] {return !_queue.empty();});

    // pull the message from the queue using move semantics
    T msg = std::move(_queue.back());
    // clear the message queue
    _queue.clear();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> lock(_mutex);
    // add message to the queue
    _queue.push_back(std::move(msg));
    // notify the msg is available
    _cond.notify_one();
}

/* Implementation of class "TrafficLight" */
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true)
    {
        if ( _messageQueue.receive() == TrafficLightPhase::green)
            return;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    
    // initialize random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(4.0, 6.0);
    
    // initialize cycle start time and generate random cycle time
    auto cycleStart = std::chrono::high_resolution_clock::now();
    // generate a random number between 4-6s
    double cycleDuration = dist(mt);
    // initialize time elapsed
    std::chrono::duration<double> timeElapsed;

    while (true)
    {
        // sleep 1ms between 2 cycles
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timeElapsed = std::chrono::high_resolution_clock::now() - cycleStart;

        if (timeElapsed.count() > cycleDuration)
        {
            // toggle light phase
            _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
            // send updated status to message queue using move semantics
            _messageQueue.send(std::move(_currentPhase));

            // update cycle start
            cycleStart = std::chrono::high_resolution_clock::now();
            // regenerate random cycle duration between 4-6s
            cycleDuration = dist(mt);
        }
    }

}

