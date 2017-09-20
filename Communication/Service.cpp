#include "Service.h"

void Service::SetConnected() {
    m_connectedEvent.Set();
}
