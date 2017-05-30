/* Copyright (c) 2017 Tallence AG and the authors, see the included COPYING file */

#ifndef SRC_LIBRMB_RADOS_STORAGE_H_
#define SRC_LIBRMB_RADOS_STORAGE_H_

#include <list>
#include <string>
#include <cstdint>

#include <rados/librados.hpp>

namespace tallence {
namespace librmb {

class RadosStorage {
 public:
  RadosStorage(librados::IoCtx* ctx, const std::string& username, const std::string& oid);
  virtual ~RadosStorage();

  const std::string& get_oid() const { return oid; }

  const std::string& get_username() const { return username; }

  librados::IoCtx& get_io_ctx() { return io_ctx; }

 private:
  librados::IoCtx io_ctx;
  std::string oid;
  std::string username;
};

}  // namespace librmb
}  // namespace tallence

#endif  // SRC_LIBRMB_RADOS_STORAGE_H_