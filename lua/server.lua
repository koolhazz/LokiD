require("lua/flag")

local mysql = 		require("lib/mysql")
local redis_ffi = 	require("lib/redis_ffi")
local logger = 		require("lua/logger")
local config = 		require("lua/config")
local stdlib = 		require("lib/stdlib")

local G = _G
local log = log
local table = table
local insert = table.insert
local concat = table.concat

local __redis = nil
local __mysql = nil
local __tid = nil

-- cache中获取用户的宝石数量
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

-- 回写用户宝石记录到数据库中
local function __WriteUserGem(in_n_userid, in_n_gem)
	local __sql = {"call p_upd_user_gem("}
	
	insert(__sql, in_n_userid)
	insert(__sql, ", "..in_n_gem)
	insert(__sql, ")")

	local __temp = concat(__sql, nil)
	
	logger.debug(string.format("W:[%d] SQL: %s", __tid, __temp));
	
	call(__mysql, __temp)
end

-- 处理逻辑开始处理
local function __start()
	__redis = redis_ffi.RedisFFI:NEW()

	if __redis:CONNECT(config.REDIS_CONF.m_s_host, config.REDIS_CONF.m_n_port) then
		while true do
			local __result = __redis:LPOP("USER_GEM_Q")
			
			if __result then
				local __gem = __GetUserGem(tonumber(__result))

				__WriteUserGem(__result, __gem)
			else
				stdlib.usleep(100)
			end
		end
	end
end

function start(in_n_tid)
	__mysql = connect()

	__tid = in_n_tid

	if __mysql then
		log.debug("mysql connect success.")
	else
		log.error("mysql connect failed.")
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

function connect()
	return mysql.connect(config.MYSQL_CONF.m_host, config.MYSQL_CONF.m_user, nil, config.MYSQL_CONF.m_db, 'utf8', 3388)
end


function query(in_t_mysql, in_s_sql)
	
end

function call(in_t_mysql, in_s_sql)
	in_t_mysql:query(in_s_sql)
end

