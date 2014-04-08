module(..., package.seeall)

local ffi = require("ffi")

ffi.cdef[[
	unsigned int sleep(unsigned int seconds);
	unsigned int usleep(unsigned long usec);
]]

local C = ffi.C

function sleep(in_n_sec)
	C.sleep(in_n_sec)
end

function usleep(in_n_usec)
	C.usleep(in_n_usec)
end
