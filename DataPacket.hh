#ifndef _DATA_PACKET_HH_
#define _DATA_PACKET_HH_

#include <memory>
#include <string>

struct data_packet{
  data_packet() : clock_counter(0), header_time(0) {}
  data_packet(std::u32string s, uint32_t ht, long cc) :
      buff(std::move(s)), clock_counter(cc), header_time(ht) {}
  data_packet(const data_packet& rhs)=delete;
  data_packet(data_packet&& rhs) : buff(std::move(rhs.buff)),
      clock_counter(rhs.clock_counter), header_time(rhs.header_time), digi(rhs.digi) {}
  ~data_packet() {reset();}

  data_packet& operator=(const data_packet& rhs)=delete;
  data_packet& operator=(data_packet&& rhs) {
    buff=std::move(rhs.buff);
    clock_counter=rhs.clock_counter;
    header_time=rhs.header_time;
    digi=rhs.digi;
    return *this;
  }

  void reset() {buff.clear(); digi.reset();}
  std::u32string buff;
  long clock_counter;
  uint32_t header_time;
  std::shared_ptr<V1724> digi;
};

#endif // _DATA_PACKET_HH_ defined
