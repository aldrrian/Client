#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "src/sockets/Udp.h"
#include <unistd.h>
#include "src/TripInfo.h"
#include "src/Driver.h"
#include "src/Grid.h"
#include "src/StandartCab.h"
#include "src/LuxuryCab.h"



using namespace std;
using namespace boost::archive;
std::stringstream ss;

//buffer to string method
std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);
    return ret;
}
//Client main
int main(int argc, char *argv[]) {
    int vehicle_id, age, exp, driver_id; //Create a Driver and send request
    char status, space;//to server for the desired cab
    cin >> driver_id >> space >> age >> space >> status
        >> space >> exp >> space >> vehicle_id;
    Driver *d = new Driver(driver_id, age, status,
                           exp, vehicle_id);
    Grid *g;
    int PortNumb = atoi(argv[2]);
    Udp udp(0, PortNumb);
    string ip = argv[1];
    udp.setIp(ip);
    udp.initialize();
    char buffer[10000];
    char buffer2[11264];
    char buffer3[4096];

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string>
            inserter(serial_str);
    boost::iostreams::stream<boost::iostreams
    ::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << d;
    s.flush();
    udp.sendData(serial_str);//send serialized driver
    udp.reciveData(buffer3, sizeof(buffer3));//recieve cab
    char check = buffer3[0];
    if (check == '1') {//differ between desired cabs
        udp.reciveData(buffer, sizeof(buffer));
        string str = bufferToString(buffer, sizeof(buffer));
        StandartCab *c;
        boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
        boost::archive::binary_iarchive ia(s);
        ia >> c;
        d->setCab(c);
        //s.flush();
    }
    else {
        udp.reciveData(buffer, sizeof(buffer));
        string str = bufferToString(buffer, sizeof(buffer));
        LuxuryCab *c;
        boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
        boost::archive::binary_iarchive ia(s);
        ia >> c;
        d->setCab(c);
    }
    udp.reciveData(buffer2, sizeof(buffer2));//recieve grid
    string str2 = bufferToString(buffer2, sizeof(buffer2));
    boost::iostreams::basic_array_source<char> device2(str2.c_str(), str2.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s3(device2);
    boost::archive::binary_iarchive ia2(s3);
    ia2 >> g;
    //s3.flush();
    d->setLocation(g->root());
    bool exit = false;
    TripInfo *ti = NULL;
    while (!exit) {
        char buffer4[4096];
        udp.reciveData(buffer4, sizeof(buffer4));
        switch (buffer4[0]) {
            case '4': {
                udp.sendData(d->getLocation()->toString());
                break;
            }
            case '7': {
                exit = true;
                delete d;
                delete g;
                break;
            }
            case '9': {
                d->drive();
                if (!d->getLocation()->equal(ti->getEnd())) {
                    udp.sendData("1");
                } else {
                    udp.sendData("0");
                }
                break;
            }
            default: {

                if (ti != NULL) {
                    delete ti;
                }
                serial_str = bufferToString(buffer4, sizeof(buffer4));
                boost::iostreams::basic_array_source<char> device
                        (serial_str.c_str(), serial_str.size());
                boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
                boost::archive::binary_iarchive ia(s2);
                ia >> ti;
                d->setTripInfo(ti);
                break;
            }
        }
    }
    udp.~Udp();
}