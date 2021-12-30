//*************************************************************************************
//      Copyright (C) 2014-2016 Canon Medical Systems Corporation. All rights reserved.
//*************************************************************************************
#pragma once

#include <string>
#include <stdint.h>
#include <array>

class md5_hash
{
public:
	md5_hash();
	md5_hash& append(const uint8_t *initial_msg, size_t initial_len);
	std::string str() const;
	std::string short_str() const;
	std::array<int,4> digest() const;

private:
	uint32_t h0,h1,h2,h3;
};


