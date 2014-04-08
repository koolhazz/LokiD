module(..., package.seeall)

local ffi = require("ffi")

ffi.cdef[[
	unsigned long compressBound(unsigned long sourceLen);
	int compress2(uint8_t *dest, unsigned long *destLen, const uint8_t *source, unsigned long sourceLen, int level);
	int uncompress(uint8_t *dest, unsigned long *destLen, const uint8_t *source, unsigned long sourceLen);
]]

local zlib = ffi.load("z")
local C = ffi.C
local setmetatable = setmetatable
local assert = assert 

local _default_buff_len = 64 * 1024
local _default_level = -1

zlibffi = {
	m_n_level = _default_level,
	m_n_buff_len = _default_buff_len,
	m_s_version = "1.0.0",
}

function zlibffi:new(o)
	o = o or {}

	setmetatable(o, self)

	self.__index = self

	return o
end

function zlibffi:compress(in_s_txt)
	print(self.m_n_buff_len)
	local buf = ffi.new("uint8_t[?]", self.m_n_buff_len)
	local buflen = ffi.new("unsigned long[1]", self.m_n_buff_len)
	local res = zlib.compress2(buf, buflen, in_s_txt, #in_s_txt, self.m_n_level)
	assert(res == 0)
	return ffi.string(buf, buflen[0])
end

function zlibffi:compresslevel(in_s_txt, in_n_level)
	local buf = ffi.new("uint8_t[?]", self.m_n_buff_len)
	local buflen = ffi.new("unsigned long[1]", self.m_n_buff_len)
	local res = zlib.compress2(buf, buflen, in_s_txt, #in_s_txt, in_n_level)
	assert(res == 0)
	return ffi.string(buf, buflen[0])
end

function zlibffi:uncompress(in_s_txt)
	local buf = ffi.new("uint8_t[?]", self.m_n_buff_len)
	local buflen = ffi.new("unsigned long[1]", self.m_n_buff_len)
	local res = zlib.uncompress(buf, buflen, in_s_txt, #in_s_txt)
	assert(res == 0)
	return ffi.string(buf, buflen[0])
end

function zlibffi:setbuffsize(in_n_size)
	self.m_n_buff_len = in_n_size or self.m_n_buff_len
end

function zlibffi:getbuffsize()
	return self.m_n_buff_len
end

function zlibffi:setlevel(in_n_level)
	self.m_n_level = in_n_level or self.m_n_level
end

function zlibffi:getlevel()
	return self.m_n_level
end

function zlibffi:version()
	return self.m_s_version
end