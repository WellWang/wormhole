/**
 * \file   progress.h
 * \brief The algorithm progress a worker/server report to the scheduler. It
 * should be printable, serializable, and mergable.
 */
#pragma once
#include <string>
#include <vector>
#include "dmlc/io.h"
#include "dmlc/logging.h"
namespace dmlc {

/**
 * \brief  The interface of progress
 */
class IProgress : public Serializable {
 public:
  IProgress() { }
  virtual ~IProgress() { }

  virtual void Clear() = 0;
  virtual bool Empty() const = 0;

  /// merger from another progress
  virtual void Merge(const IProgress* const other) = 0;

  /// head string for printing
  virtual std::string HeadStr() = 0;

  /// string for printing
  virtual std::string PrintStr(const IProgress* const prev) = 0;
};


class VectorProgress : public IProgress {
 public:
  VectorProgress() { Resize(0, 0); }
  VectorProgress(int inum, int fnum) { Resize(inum, fnum); }
  virtual ~VectorProgress() { }

  void Resize(int inum, int fnum) {
    fvec_.resize(fnum);
    ivec_.resize(inum);
  }

  virtual bool Empty() const {
    for (double f : fvec_) if (f != 0) return false;
    for (size_t i : ivec_) if (i != 0) return false;
    return true;
  }

  virtual void Clear() { Resize(0, 0); }


  virtual void Load(Stream* fi) {
    fi->Read(&fvec_);
    fi->Read(&ivec_);
  }

  virtual void Save(Stream *fo) const {
    fo->Write(fvec_);
    fo->Write(ivec_);
  }

 protected:
  std::vector<double> fvec_;
  std::vector<size_t> ivec_;
};

}  // namespace dmlc
