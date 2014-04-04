require("script/global")
require("script/protocal")

local config = require("script/config")
local db = require("script/db")
local game_table = require("script/game_table")
local game_report = require("script/game_report")
local hall = require("script/hall")
local match = require("script/match_server")
require("script/server_timer")
local gem = require("script/gem")

local log = log
local l_t_timer_map = g_t_timer_map
local l_t_socket_map = g_t_socket_map
local l_t_disconnect_user_map = g_t_disconnect_user_map
local l_t_user_map = g_t_user_map
local redis = redis
local server = server

local G = _G

local function __init_gem(in_n_level)
	log.debug("-------- __init_gem begin --------")
	gem.Array(gem.__gem_rnd_array, in_n_level, gem.__task_info)

	log.debug("array: "..#gem.__gem_rnd_array)
	log.debug("-------- __inti_gem end --------")
end

function handle_init()
	local l_n_port = global_command_args["p"]
	local l_s_server_ip = global_command_args["h"]
	local l_n_level = tonumber(global_command_args["l"])
	local l_n_sid 	= global_command_args["s"]
	
   	if -1 == hall.connect() then
    	return -1
    end

    log.debug("hall connect success.")
    
	--[[
	if -1 == match.connect() then
    	return -1
    end
    log.debug("match connect success.")
    ]]--

    local l_n_redis = redis.connect_redis(config.REDIS_CONF.m_s_host, config.REDIS_CONF.m_n_port, config.REDIS_CONF.m_n_timeout)
   
   	if (l_n_redis == 0) then
   	   	log.debug("redis connect success.")
   	end

    if -1 == db.connect_mysql_svr() then
    	log.debug("mysql connect failed.")
        return -1
    else
    	log.debug("mysql connect success.")
    end
	
	db.reset_member_online(l_n_sid)

    local l_ret = server.create_listener(l_n_port)
	if l_ret == -1 then 
		log.error("create listen socket failed, port=" .. l_n_port)
		return -1
	end

	--game_report.start_report_memory_timer()
	--game_report.start_report_user_timer()

	__init_gem(l_n_level) -- 初始化宝石系统
	
	dump(false)

	log.info("Server init success")
	
	return 0
end

function handle_fini()
	log.info("Server will stop")
	return 0
end

function handle_accept(socket)
	log.debug("New Connection: "..socket)

	-- start timer
	-- start_conn_timer(socket)		

	return 0	
end

function handle_input(socket, buffer, length)
	log.debug("Recv buffer: "..buffer)
	return 0
end

function handle_server_socket_close(socket, in_nconn_type)
	log.debug("Remote server socket has closed, socket = " 
    .. socket ..", connection type = " .. in_nconn_type)

	if G["g_n_hall_socket"] == socket then
		hall.reconnect()
	end

	if G["g_n_match_socket"] == socket then
		match.reconnect()
	end

	return 0
end

function handle_client_socket_close(in_n_socket)
	log.debug("Client socket has closed: " .. in_n_socket)
	
	local l_n_timer_id = G["g_t_conn_timer_map"][in_n_socket]
	
	if l_n_timer_id ~= nil then
		stop_conn_timer(in_n_socket)
	end
	
	local l_n_user_id = l_t_socket_map[in_n_socket]
	
	if l_t_disconnect_user_map[l_n_user_id] ~= nil then
		log.debug("Client socket has in g_t_disconnect_list "..l_n_user_id)

		local l_t_user = l_t_user_map[l_n_user_id]

		if nil == l_t_user then
			log.debug("Client socket user nil "..l_n_user_id)
			return 0
		end

		l_t_user.m_n_socket = 0
		l_t_socket_map[in_n_socket] = nil

		return 0
	end
	
	if l_n_user_id ~= nil then
		log.debug("Client socket has closed uid: "..l_n_user_id)
        game_table.user_off_line(l_n_user_id)
	end
	
	return 0
end

function handle_timeout(in_ntimerid)
    log.debug(" ------------------ handle timeout begin timer_id = ".. in_ntimerid.."-----------------")
    local l_timer = l_t_timer_map[in_ntimerid]
    
    if nil ~= l_timer then
        --  add current timer_id
		if l_timer.m_t_params ~= nil then
			table.insert(l_timer.m_t_params, in_ntimerid)
		end

		l_timer.m_f_callback(l_timer.m_t_params)
        timer.stop_timer(in_ntimerid)
        timer.clear_timer(in_ntimerid)
        l_t_timer_map[in_ntimerid] = nil
    else
    	log.debug("Timer is not found: "..in_ntimerid)
    end
    
    log.debug(" ------------------ handle timeout end timer_id = ".. in_ntimerid.."-----------------")
	return 0
end

function reload_config()
	log.debug("-------- reload_config begin --------")
	package.loaded["script/global"] = nil
	require("script/global")

	if g_flag_reload then
		log.debug("reload true")
	else
		log.debug("reload false")
	end
	
	log.debug("-------- reload_config end --------")
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
