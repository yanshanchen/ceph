// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#ifndef CEPH_MDSOPENINO_H
#define CEPH_MDSOPENINO_H

#include "msg/Message.h"

class MMDSOpenIno : public Message {
  static const int HEAD_VERSION = 1;
  static const int COMPAT_VERSION = 1;
public:
  inodeno_t ino;
  vector<inode_backpointer_t> ancestors;

protected:
  MMDSOpenIno() : Message{MSG_MDS_OPENINO, HEAD_VERSION, COMPAT_VERSION} {}
  MMDSOpenIno(ceph_tid_t t, inodeno_t i, vector<inode_backpointer_t>* pa) :
    Message{MSG_MDS_OPENINO, HEAD_VERSION, COMPAT_VERSION}, ino(i) {
    header.tid = t;
    if (pa)
      ancestors = *pa;
  }
  ~MMDSOpenIno() override {}

public:
  std::string_view get_type_name() const override { return "openino"; }
  void print(ostream &out) const override {
    out << "openino(" << header.tid << " " << ino << " " << ancestors << ")";
  }

  void encode_payload(uint64_t features) override {
    using ceph::encode;
    encode(ino, payload);
    encode(ancestors, payload);
  }
  void decode_payload() override {
    auto p = payload.cbegin();
    decode(ino, p);
    decode(ancestors, p);
  }
private:
  template<class T, typename... Args>
  friend boost::intrusive_ptr<T> ceph::make_message(Args&&... args);
};

#endif
