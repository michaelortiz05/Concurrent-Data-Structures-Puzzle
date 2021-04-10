#include "Servant.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

// Each servant will keep working until the bag is empty
void doWork(Servant &s) {
    while (s.getBagSize() > 0) {
        auto tag = s.getPresentFromBag();
        if (tag == -1) break;
        s.writeThankyou(tag);
    }
}

// Generate 60 temperature readings
void simulateTemperatureReading(LockDeque& l) {
    srand(time(NULL));
    for (int i = 1; i <= 60; i++) {
        int reading = rand() % 171 - 100;
        TempReading* tr = new TempReading;
        tr->temperature = reading;
        tr->minute = i;
        l.push_back(tr);
        this_thread::sleep_for (chrono::milliseconds(10));
    }
}

// Execute problem 1
void problem1() {
    const int numPresents = 500000;
    ConcurrentLinkedList l;
    ConcurrentBag bag(numPresents);

    Servant s1(l, bag);
    Servant s2(l, bag);
    Servant s3(l, bag);
    Servant s4(l, bag);

    // Have all 4 servants work on the presents in parallel
    thread t1(doWork, ref(s1));
    thread t2(doWork, ref(s2));
    thread t3(doWork, ref(s3));
    thread t4(doWork, ref(s4)); 

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    ofstream f;
    f.open("results-problem1.txt");
    f << "Servant | # presents | # thank-yous\n";
    f << "1\t\t" << s1.getNumPresents() << "\t\t " << s1.getNumThankyous() << "\n"; 
    f << "2\t\t" << s2.getNumPresents() << "\t\t " << s2.getNumThankyous() << "\n"; 
    f << "3\t\t" << s3.getNumPresents() << "\t\t " << s3.getNumThankyous() << "\n"; 
    f << "4\t\t" << s4.getNumPresents() << "\t\t " << s4.getNumThankyous() << "\n"; 
    f << "\nTotal presents: " << s1.getNumPresents() + s2.getNumPresents() + s3.getNumPresents() + s4.getNumPresents() << "\n";
    f.close();

    cout << "Results written to \'results-problem1.txt\'" << endl;  
}

// Execute problem 2
void problem2() {

    // Simulate 10 hours
    const int numHours = 10;
    for (int i = 0; i < numHours; i++) {

        // Prepare necessary data structures
        LockDeque readings;
        priority_queue <int> max_heap;
        priority_queue <int, vector<int>, greater<int> > min_heap;
        unordered_map<int, int> tenMinuteTemps;
        vector<thread> sensors;

        // Grab readings from 8 sensors and simulate 1 hour
        for (int i = 0; i < 8; i++)
            sensors.push_back(thread(simulateTemperatureReading, ref(readings)));

        this_thread::sleep_for (chrono::milliseconds(600));
        for (auto &t : sensors)
            t.join();

        // Parse data using a minheap, maxheap, and hashmap and report results
        while (readings.size() > 0) {
            auto tr = readings.pop_front();
            min_heap.push(tr->temperature);
            max_heap.push(tr->temperature);
            auto minute = tr->minute;
            if (minute == 1 || minute == 10 || minute == 11 || minute == 20 || minute == 21 || 
                minute == 30 || minute == 31 || minute == 40 || minute == 41 || minute == 50 || minute == 51 || minute == 60) {

                if (tenMinuteTemps.find(minute) != tenMinuteTemps.end()) {
                    if (minute % 2 == 0) 
                        tenMinuteTemps[minute] = tenMinuteTemps.at(minute) > tr->temperature ? tenMinuteTemps.at(minute) : tr->temperature;
                    else
                        tenMinuteTemps[minute] = tenMinuteTemps.at(minute) < tr->temperature ? tenMinuteTemps.at(minute) : tr->temperature;
                }

                else {
                    tenMinuteTemps[minute] = tr->temperature;
                }
            }
        }  
        cout << "RESULTS FOR HOUR " << i << endl;
        cout << "Five highest temperatures: ";
        for (int i = 0; i < 5; i++) {
            cout << max_heap.top() << " ";
            max_heap.pop();
        }      
        cout << endl;
        cout << "Five lowest temperatures: ";
        for (int i = 0; i < 5; i++) {
            cout << min_heap.top() << " ";
            min_heap.pop();
        }   
        cout << endl;
        cout << "Largest temperature difference: ";
        int largestDiff = 0;
        for(int i = 1; i <= 60; i += 10) {
            int diff = abs(tenMinuteTemps.at(i + 9) - tenMinuteTemps.at(i));
            largestDiff = diff > largestDiff ? diff : largestDiff;
        }
        cout << largestDiff << "\n" << endl;

    }
}

// Program menu
int main() {
    int selection = 0;
    while (selection != 1 || selection != 2) {
        cout << "Enter 1 to run problem 1, 2 to run problem 2" << endl;
        cin >> selection;
        switch(selection) {
            case 1:
                problem1();
                return 0;
            case 2:
                problem2();
                return 0;
            default:
                break;
        }
    }
}

