--[[
	Desc: 	配置信息模块
	Date:	2012.09.14
	Auth:	AustinChen
]]--

module(..., package.seeall)

MYSQL_CONF = {
	m_host 		= "192.168.100.167",
	m_port 		= 3388,
	m_db 		= "kslave",
	m_user 		= "root",
	m_password 	= "",	
}

HALL_SVR_CONF = 
{
	m_host = "192.168.100.141",
	m_port = 3333,
}

MATCH_SVR_CONF =
{
    m_host = "10.66.46.29",
    m_port = 8500,
}

BACK_SVR_CONF = 
{
	m_host = "192.168.100.30",
	m_port = 12345,
}

SERVER_CONF = 
{
    m_min_level = 1,
    m_max_level = 50,
}

REDIS_CONF = {
	m_s_host = 		"192.168.100.167",
	m_n_port = 		4501,
	m_n_timeout = 	5,
}
