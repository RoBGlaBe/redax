#ifndef _STRAXINSERTER_HH_
#define _STRAXINSERTER_HH_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <map>
#include <mutex>
#include <experimental/filesystem>
#include <numeric>
#include <atomic>
#include <vector>
#include <thread>
#include <condition_variable>
#include <list>
#include <memory>
#include <string_view>
#include "DataPacket.hh"

class Options;
class MongoLog;
class V1724;

class StraxFormatter{
  /*
    Reformats raw data into strax format
  */

public:
  StraxFormatter(std::shared_ptr<Options>&, std::shared_ptr<MongoLog>&);
  ~StraxFormatter();

  void Close(std::map<int,int>& ret);

  void Process();
  std::pair<int, int> GetBufferSize() {return {fInputBufferSize.load(), fOutputBufferSize.load()};}
  void GetDataPerChan(std::map<int, int>& ret);
  void ReceiveDatapackets(std::list<data_packet>&, int);

private:
  void ProcessDatapacket(data_packet dp);
  int ProcessEvent(std::u32string_view, const data_packet&,
      std::map<int, int>&);
  int ProcessChannel(std::u32string_view, int, int, uint32_t, int&, int,
      const data_packet&, std::map<int, int>&);
  void WriteOutChunk(int);
  void WriteOutChunks();
  void End();
  void GenerateArtificialDeadtime(int64_t, const std::shared_ptr<V1724>&);
  void AddFragmentToBuffer(std::string, uint32_t, int);
  std::vector<std::string> GetChunkNames(int);

  std::experimental::filesystem::path GetFilePath(const std::string&, bool=false);
  std::experimental::filesystem::path GetDirectoryPath(const std::string&, bool=false);
  std::string GetStringFormat(int id);
  void CreateEmpty(int);
  int fEmptyVerified;

  int64_t fChunkLength; // ns
  int64_t fChunkOverlap; // ns
  int fFragmentBytes;
  int fStraxHeaderSize; // bytes
  int fFullFragmentSize;
  int fBufferNumChunks;
  int fWarnIfChunkOlderThan;
  unsigned fChunkNameLength;
  int64_t fFullChunkLength;
  std::string fOutputPath, fHostname, fFullHostname;
  std::shared_ptr<Options> fOptions;
  std::shared_ptr<MongoLog> fLog;
  std::atomic_bool fActive;
  std::string fCompressor;
  std::map<int, std::list<std::string>> fChunks, fOverlaps;
  std::map<int, int> fFailCounter;
  std::map<int, int> fDataPerChan;
  std::mutex fDPC_mutex;
  std::map<int, long> fBufferCounter;
  std::map<int, long> fFragsPerEvent;
  std::map<int, long> fEvPerDP;
  std::map<int, long> fBytesPerChunk;
  std::atomic_int fInputBufferSize, fOutputBufferSize;
  long fBytesProcessed;

  double fProcTimeDP, fProcTimeEv, fProcTimeCh, fCompTime;
  std::thread::id fThreadId;
  std::condition_variable fCV;
  std::mutex fBufferMutex;
  std::list<data_packet> fBuffer;
};

#endif
