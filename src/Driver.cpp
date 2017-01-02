/*
 * Driver.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: shmuel
 */

#include "Driver.h"

namespace std {
    Driver::Driver(int identity, int a, char ms, int e, int vId, BFSPoint* r) {
        clients = 0;
        satisfaction = 0;
        id = identity;
        age = a;
        exp = e;
        marist = ms;
        maritalStatus = myMaritalStatus(marist);
        cab = NULL;
        vehicleId = vId;
        currentLocation = r;
        haveAPassengers = false;
        amIOccupied = false;
        myPassenger = NULL;
    }
    Driver::Driver(int identity, int a, char ms, int e, int vId) {
        clients = 0;
        satisfaction = 0;
        id = identity;
        age = a;
        exp = e;
        marist = ms;
        maritalStatus = myMaritalStatus(marist);
        cab = NULL;
        vehicleId = vId;
        currentLocation = NULL;
        haveAPassengers = false;
        amIOccupied = false;
        myPassenger = NULL;
    }
    Driver::Driver() {
        clients = 0;
        satisfaction = 0;
        id = 0;
        age = 0;
        exp = 0;
        cab = NULL;
    }
    Driver::~Driver() {
        delete  cab;
    }
    int Driver::getId() {
        return id;
    }
    int Driver::getAge(){
        return age;
    }
    int Driver::getExp() {
        return  exp;
    }
    int Driver::getClients() {
        return clients;
    }
    double Driver::getStisfaction() {
        return satisfaction;
    }
    Cab* Driver::getCab() {
        return cab;
    }
    void Driver::setSatisfaction(double st) {
        memo += st;
        satisfaction = memo / clients;
    }
    void Driver::setCab(Cab* c) {
        cab = c;
        cab->setMyLocation(currentLocation);
    }
    BFSPoint* Driver::getLocation() {
        return currentLocation;
    }
    int Driver::findShortPath(TripInfo* ti) {
        Bfs b;
        return (int)b.path(currentLocation,ti->getStart()).size();

    }
    void Driver::drive(){
        if (currentLocation->equal(tripInfo->getStart())) {
            pickUp();
        }
        setLocation(cab->drive(&myWay));
        if (currentLocation->equal(tripInfo->getEnd())) {
            dropOff();
        }
    }
    MaritalStatus Driver::getMaritalStatus() {
        return maritalStatus;
    }
    void Driver::setTripInfo(TripInfo * ti) {
        tripInfo = ti;
        road();
    }
    MaritalStatus Driver::myMaritalStatus(char ms) {
        switch (ms) {
            case 'W':
                return MaritalStatus::WIDOWED;
            case 'D':
                return MaritalStatus::DIVORCED;
            case 'S':
                return MaritalStatus::SINGLE;
            case 'M':
                return MaritalStatus::MARRIED;
            default:
                return MaritalStatus ::DEFAULT;
        }
    }
    int Driver::myVehicleId() {
        return vehicleId;
    }
    void Driver::road() {
        Bfs b;
        if (currentLocation->equal(tripInfo->getStart())) {
            myWay = b.path(currentLocation, tripInfo->getEnd());
            tripInfo->setMeter(myWay.size());
            cab->setKiloPassed(myWay.size());
        } else {
            myWay = b.path(currentLocation, tripInfo->getStart());
            if (! currentLocation->equal(tripInfo->getStart())) {
                cab->setKiloPassed(myWay.size());
            }
        }
    }
    void Driver::pickUp() {
        clients++;
        road();
    }
    void Driver::dropOff() {
        setSatisfaction(myPassenger->randSatisfaction());
        haveAPassengers = false;
        setOccupied();
        //collect money.
    }
    void Driver::setOccupied() {
        amIOccupied = !amIOccupied;
    }
    bool Driver::occupied() {
        return amIOccupied;
    }
    char Driver::myMaritst() {
        return marist;
    }
    void Driver::setLocation(BFSPoint *loc) {
        currentLocation = loc;
    }

} /* namespace std */