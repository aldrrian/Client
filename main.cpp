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


using namespace std;
using namespace boost::archive;
std::stringstream ss;

std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);

    return ret;
}
int main(int argc, char *argv[]) {
    int vehicle_id, age, exp, driver_id;
    char status, space;
    cin >> driver_id >> space >> age >> space >> status
        >> space >> exp >> space >> vehicle_id;
    Driver d(driver_id, age, status,
                            exp, vehicle_id);
    int PortNumb = 5555;
    Udp udp(0, PortNumb);
    udp.initialize();
    char buffer[1024], buffer2[1024];
    udp.sendData(to_string(vehicle_id));
    udp.reciveData(buffer, sizeof(buffer));
    string str = bufferToString(buffer, sizeof(buffer));
    StandartCab *c;
    boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    boost::archive::binary_iarchive ia(s);
    ia >> c;
    cout << "("<<c->getID()<<")"<<endl;
    udp.reciveData(buffer2, sizeof(buffer2));
    str = bufferToString(buffer2, sizeof(buffer2));
    Grid *g;
    boost::iostreams::basic_array_source<char> device2(str.c_str(), str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device2);
    boost::archive::binary_iarchive ia2(s2);
    ia2 >> g;
    cout << "("<<g->root()->getX()<<")"<<endl;

}


