// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Copyright (c) 2017-2018 Tallence AG and the authors
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 */

#ifndef SRC_LIBRMB_RADOS_MAIL_H_
#define SRC_LIBRMB_RADOS_MAIL_H_

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "rados-metadata.h"
#include "rados-types.h"
#include <rados/librados.hpp>

namespace librmb {

using std::string;
using std::map;

using librados::AioCompletion;
using librados::ObjectWriteOperation;

/**
 * Rados mail object
 *
 * ceph mail representation.
 *
 */
class RadosMail {
 public:
  RadosMail();
  virtual ~RadosMail();
  void set_oid(const char* _oid) { this->oid = _oid; }
  void set_oid(const string& _oid) { this->oid = _oid; }
  void set_guid(const uint8_t* guid);
  void set_mail_size(const uint64_t& _size) { object_size = _size; }
  void set_active_op(bool _active) { this->active_op = _active; }
  void set_rados_save_date(const time_t& _save_date) { this->save_date_rados = _save_date; }

  const string& get_oid() { return this->oid; }
  const uint64_t& get_mail_size() { return this->object_size; }

  time_t* get_rados_save_date() { return &this->save_date_rados; }
  uint8_t* get_guid_ref() { return this->guid; }
  /*!
   * @return ptr to internal buffer .
   */
  librados::bufferlist* get_mail_buffer() { return &this->mail_buffer; }
  map<string, ceph::bufferlist>* get_metadata() { return &this->attrset; }

  /*!
   * @return reference to all write operations related with this object
   */
  map<AioCompletion*, ObjectWriteOperation*>* get_completion_op_map() { return &completion_op; }

  /*!
   * get metadata value
   * @param[in] key rbox_metadata_key
   * @param[out] value ptr to valid std::string buffer.
   */
  void get_metadata(rbox_metadata_key key, std::string* value) {
    string str_key(librmb::rbox_metadata_key_to_char(key));
    get_metadata(str_key, value);
  }

  void get_metadata(const string& key, std::string* value) {
    if (attrset.find(key) != attrset.end()) {
      *value = attrset[key].to_str();
    }
  }

  bool is_index_ref() { return index_ref; }
  void set_index_ref(bool ref) { this->index_ref = ref; }
  bool is_valid() { return valid; }
  void set_valid(bool valid_) { valid = valid_; }
  bool has_active_op() { return active_op; }
  string to_string(const string& padding);
  void add_metadata(const RadosMetadata& metadata) { attrset[metadata.key] = metadata.bl; }

  /*!
   * Some metadata isn't saved as xattribute (default). To access those, get_extended_metadata can
   * be used.
   */
  map<string, ceph::bufferlist>* get_extended_metadata() { return &this->extended_attrset; }
  /*!
     * Save metadata to extended metadata store currently omap
     * @param[in] metadata valid radosMetadata.
     */
  void add_extended_metadata(RadosMetadata& metadata) { extended_attrset[metadata.key] = metadata.bl; }
  const string get_extended_metadata(string& key) {
    string value;
    if (extended_attrset.find(key) != extended_attrset.end()) {
      value = extended_attrset[key].to_str();
    }
    return value;
  }

 private:
  string oid;

  uint8_t guid[GUID_128_SIZE] = {};
  uint64_t object_size;  // byte
  map<AioCompletion*, ObjectWriteOperation*> completion_op;

  bool active_op;
  ceph::bufferlist mail_buffer;
  time_t save_date_rados;

  map<string, ceph::bufferlist> attrset;
  map<string, ceph::bufferlist> extended_attrset;
  bool valid;
  bool index_ref;

 public:
  /** the xattribute version */
  static const char X_ATTR_VERSION_VALUE[];
};

}  // namespace librmb

#endif  // SRC_LIBRMB_RADOS_MAIL_H_