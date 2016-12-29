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
#include "src/Grid.h"
#include "src/LuxuryCab.h"
using namespace std;
using namespace boost::archive;
std::stringstream ss;

std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);

    return ret;
}
int main(int argc, char *argv[]) {
    BFSPoint *st = new BFSPoint(2,1);
    BFSPoint *en = new BFSPoint(4,5);
    TripInfo *ti = new TripInfo(1,st,en,2,20);

string serial_str;
/*TripInfo *ti2;
boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
boost::archive::binary_iarchive ia(s2);
ia >> ti2;
    cout << "("<<ti2->getEnd()->getX()<<","<<ti2->getEnd()->getY()<<")"<<endl;*/
cout << argv[1] << endl;
    Udp udp(0, atoi(argv[1]));
    udp.initialize();
    char buffer[1024];
    /*udp.sendData("lol");
    udp.reciveData(buffer, sizeof(buffer));*/
    Grid *g = new Grid(7,8);
    LuxuryCab *sc = new LuxuryCab(8, 'H', 'B');
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << sc;
    s.flush();
    TripInfo *ti2;
    Point *po2;
    BFSPoint *yt;
    Grid *g2;
    LuxuryCab *sc2;
    boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> sc2;
    cout<< sc2->getID()<<endl;


    return 0;
}


