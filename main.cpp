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
#include "/home/shmuel/ClionProjects/newHope/TripInfo.h"
using namespace std;
using namespace boost::archive;
std::stringstream ss;

std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);

    return ret;
}
asfx,cjnalxvjselfnxzoihnas,dixubas,md nkj
int main(int argc, char *argv[]) {
    BFSPoint *st = new BFSPoint(2,1);
    BFSPoint *en = new BFSPoint(4,5);
    TripInfo *ti = new TripInfo(1,st,en,2,20);

string serial_str;
boost::iostreams::back_insert_device<std::string> inserter(serial_str);
boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
boost::archive::binary_oarchive oa(s);
oa << ti;
s.flush();

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
    udp.sendData("lol");
    cout << serial_str << endl;
    udp.reciveData(buffer, sizeof(buffer));
    string str = bufferToString(buffer, sizeof(buffer));
    TripInfo *ptr;
    boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> ptr;
    cout << "("<<ptr->getEnd()->getX()<<","<<ptr->getEnd()->getY()<<")"<<endl;


    return 0;
}


