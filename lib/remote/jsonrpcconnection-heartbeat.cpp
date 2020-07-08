/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#include "remote/jsonrpcconnection.hpp"
#include "remote/messageorigin.hpp"
#include "remote/apifunction.hpp"
#include "base/initialize.hpp"
#include "base/configtype.hpp"
#include "base/logger.hpp"
#include "base/utility.hpp"
#include <boost/asio/spawn.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/system/system_error.hpp>

using namespace icinga;

REGISTER_APIFUNCTION(Heartbeat, event, &JsonRpcConnection::HeartbeatAPIHandler);

void JsonRpcConnection::HandleAndWriteHeartbeats(boost::asio::yield_context yc)
{
	boost::system::error_code ec;

	for (;;) {
		m_HeartbeatTimer.expires_from_now(boost::posix_time::seconds(10));
		m_HeartbeatTimer.async_wait(yc[ec]);

		if (m_ShuttingDown) {
			break;
		}

		SendMessageInternal(new Dictionary({
			{ "jsonrpc", "2.0" },
			{ "method", "event::Heartbeat" },
			{ "params", new Dictionary() }
		}));
	}
}

Value JsonRpcConnection::HeartbeatAPIHandler(const MessageOrigin::Ptr& origin, const Dictionary::Ptr& params)
{
	return Empty;
}

