#ifndef COSProfile_H
#define COSProfile_H

#include <string>
#include <COSTime.h>
#include <CStringMap.h>

class COSElapsed {
 public:
  COSElapsed(const std::string &msg) :
   msg_(msg), start_time_() {
    start_time_ = COSTime::getHRTime();
  }

 ~COSElapsed() {
    CHRTime end_time = COSTime::getHRTime();

    CHRTime dtime = COSTime::diffHRTime(start_time_, end_time);

    std::cout << msg_ << ": " << dtime << std::endl;
  }

 private:
  std::string msg_;
  CHRTime     start_time_;
};

//------

class COSTotalProfile {
 public:
  COSTotalProfile(CStringIdMap *stringMap) :
   stringMap_(stringMap), id_(), time_(), num_(0), valid_(false) {
  }

  COSTotalProfile(CStringIdMap *stringMap, const CStringId &id, const CHRTime &dtime) :
   stringMap_(stringMap), id_(id), time_(dtime), num_(1), valid_(false) {
  }

 ~COSTotalProfile() {
    if (valid_)
      std::cout << stringMap_->lookup(id_) << ": " << num_ << ": " << time_ << std::endl;
  }

  void setValid() {
    valid_ = true;
  }

  void addTime(const CHRTime &dtime) {
    time_ += dtime;

    ++num_;
  }

 private:
  CStringIdMap* stringMap_ { nullptr };
  CStringId     id_;
  CHRTime       time_;
  uint          num_   { 0 };
  bool          valid_ { false };
};

//------

class COSProfile {
 public:
  typedef std::map<CStringId,COSTotalProfile> TotalMap;

 public:
  COSProfile(const CStringId &id) :
   id_(id), start_time_() {
    start_time_ = COSTime::getHRTime();
  }

 ~COSProfile() {
    CHRTime dtime = elapsed();

    TotalMap::iterator p = totalMap_.find(id_);

    if (p != totalMap_.end())
      (*p).second.addTime(dtime);
    else {
      p = totalMap_.insert(p, TotalMap::value_type(id_, COSTotalProfile(&stringMap_, id_, dtime)));

      (*p).second.setValid();
    }
  }

  CHRTime elapsed() const {
    CHRTime end_time = COSTime::getHRTime();

    return COSTime::diffHRTime(start_time_, end_time);
  }

 private:
  static TotalMap     totalMap_;
  static CStringIdMap stringMap_;

  CStringId id_;
  CHRTime   start_time_;
};

#endif
