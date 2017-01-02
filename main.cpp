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
    int PortNumb = 5555;
    Udp udp(0, PortNumb);
    udp.initialize();
    char buffer[4096];
    char buffer2[11264];
    char buffer3[4096];
    char buffer4[4096];
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
    if (buffer3 == "1") {//differ between desired cabs
        udp.reciveData(buffer, sizeof(buffer));
        string str = bufferToString(buffer, sizeof(buffer));
        StandartCab *c;
        boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
        boost::archive::binary_iarchive ia(s);
        ia >> c;
        //s.flush();
        cout << "(" << c->getID() << ")" << endl;
    }
    else {
        udp.reciveData(buffer, sizeof(buffer));
        string str = bufferToString(buffer, sizeof(buffer));
        LuxuryCab *c;
        boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
        boost::archive::binary_iarchive ia(s);
        ia >> c;
        //s.flush();
        cout << "(" << c->getType() << ")" << endl;
    }
    udp.reciveData(buffer2, sizeof(buffer2));//recieve grid
    string str2 = bufferToString(buffer2, sizeof(buffer2));
    Grid *g;
    boost::iostreams::basic_array_source<char> device2(str2.c_str(), str2.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s3(device2);
    boost::archive::binary_iarchive ia2(s3);
    ia2 >> g;
    //s3.flush();
    d->setLocation(g->root());

    udp.reciveData(buffer4, sizeof(buffer4));//recieve tripinfo
    serial_str = bufferToString(buffer4, sizeof(buffer4));
    TripInfo *ti;
    boost::iostreams::basic_array_source<char> device
            (serial_str.c_str(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> ti;
    //s.flush();
    cout << "(" << ti->getTariff() << ")" << endl;

}


