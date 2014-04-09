module(..., package.seeall)

local log = log
local G = _G

local __level = 3 -- 日志级别

function debug(in_s_msg)
	if __level < G["g_t_log_level"].DEBUG then
		return
	else
		log.debug(in_s_msg)
	end
end

function info(in_s_msg)
	if __level < G["g_t_log_level"].INFO then
		return
	else
		log.info(in_s_msg)
	end	
end

function error(in_s_msg)
	log.error(in_s_msg)
end

function __BEGIN__(in_s_msg)
	debug("-------- "..in_s_msg.." begin --------")
end

function __END__(in_s_msg)
	debug("-------- "..in_s_msg.." end --------")	
end