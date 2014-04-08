require("lua/flag")

local db = require("lua/db")
local redis_ffi = require("lib/redis_ffi")
local logger = require("lua/logger")
local config = require("lua/config")
local stdlib = require("lib/stdlib")

local mysql = mysql
local G = _G
local log = log
local table = table


local __redis = nil

local function __GetUserGem(in_n_userid)
	if __redis then
		if __redis.IsAlived() then
			local __result = __redis:HGET("USER_GEM", tostring(in_n_userid))

			return __result and tonumber(__result) or 0
		end

		return 0
	end

	return 0
end

local function __writeUserGem(in_n_userid, in_n_gem)
	local __sql = {"call p_upd_UserGem("}

	table.insert(__sql, in_n_userid)
	table.insert(__sql, ", "..in_n_gem)
	table.insert(__sql, ")")

	local __temp = table.concat(__sql, nil)
	logger.debug("SQL: "..__temp)

	db.query(__temp)
end


local function __start()
	__redis = redis_ffi.RedisFFI:NEW()

	if __redis:CONNECT(config.REDIS_CONF.m_s_host, config.REDIS_CONF.m_n_port) then
		while true do
			local __result = __redis:LPOP("USER_GEM_Q")
			
			if __result then
				logger.debug("UserID: "..__result)

				local __gem = __GetUserGem(tonumber(__result))

				logger.debug("gem: "..__gem)

				if __gem > 0 then
					-- __WriteUserGem(__result, __gem)			
				end	
			else
				logger.error("no UserID")
				stdlib.sleep(1)	
			end
		end
	end
end

function start()
    log.debug("hall connect success.")
  
    if -1 == db.connect_mysql_svr() then
    	log.debug("mysql connect failed.")
        return -1
    else
    	log.debug("mysql connect success.")
    end
	
	dump(false)

	log.info("Server init success")

	__start()

	return 0
end

function stop()
	log.info("Server will stop")
	return 0
end

function dump(in_b_flag)
	package.path = package.path.."?.lua;../?.lua;../lib/?.lua;../../../lib/?.lua;/home/AustinChen/tools/luajit-2.0/src/?.lua;"
	if in_b_flag then
		local dump = require("jit.dump")
		dump.on(nil, "jit.log")
	else
		local dump = require("jit.v")
		dump.on("jit.log")
	end
end

