/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2020 Winlin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <srs_core.hpp>

_SrsContextId::_SrsContextId()
{
    bind_ = NULL;
    parent_ = NULL;
}

_SrsContextId::_SrsContextId(std::string v)
{
    v_ = v;
    m_ = v;
    bind_ = NULL;
    parent_ = NULL;
}

_SrsContextId::_SrsContextId(std::string k, std::string v)
{
    k_ = k;
    v_ = v;
    m_ = k + ":" + v;
    bind_ = NULL;
    parent_ = NULL;
}

_SrsContextId::_SrsContextId(const _SrsContextId& cp)
{
    k_ = cp.k_;
    v_ = cp.v_;
    m_ = cp.m_;
    pm_ = cp.pm_;
    bind_ = NULL;
    parent_ = NULL;
    if (cp.bind_) {
        bind_ = cp.bind_->copy();
    }
    if (cp.parent_) {
        parent_ = cp.parent_->copy();
    }
}

_SrsContextId& _SrsContextId::operator=(const _SrsContextId& cp)
{
    k_ = cp.k_;
    v_ = cp.v_;
    m_ = cp.m_;
    pm_ = cp.pm_;
    srs_freep(bind_);
    srs_freep(parent_);
    if (cp.bind_) {
        bind_ = cp.bind_->copy();
    }
    if (cp.parent_) {
        parent_ = cp.parent_->copy();
    }
    return *this;
}

_SrsContextId::~_SrsContextId()
{
    srs_freep(bind_);
    srs_freep(parent_);
}

const char* _SrsContextId::c_str() const
{
    if (!parent_) {
        return m_.c_str();
    }
    return pm_.c_str();
}

bool _SrsContextId::empty() const
{
    return v_.empty();
}

int _SrsContextId::compare(const _SrsContextId& to) const
{
    return m_.compare(to.m_);
}

_SrsContextId* _SrsContextId::copy() const
{
    _SrsContextId* cp = new _SrsContextId();
    cp->k_ = k_;
    cp->v_ = v_;
    cp->m_ = m_;
    cp->pm_ = pm_;
    if (bind_) {
        cp->bind_ = bind_->copy();
    }
    if (parent_) {
        cp->parent_ = parent_->copy();
    }
    return cp;
}

void _SrsContextId::bind(const _SrsContextId& target)
{
    srs_freep(bind_);
    bind_ = target.copy();
}

void _SrsContextId::with(const _SrsContextId& parent)
{
    srs_freep(parent_);
    parent_ = parent.copy();
    pm_ = std::string(parent_->c_str()) + std::string("][") + m_;
}

bool _SrsContextId::match(std::string k)
{
    if (k_ == k) {
        return true;
    }

    if (parent_) {
        return parent_->match(k);
    }

    return false;
}

