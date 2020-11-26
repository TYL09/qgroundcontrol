/****************************************************************************
 *
 * (c) 2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QString>
#include <QVector>
#include <QTimer>

#include <functional>

#include "HealthAndArmingChecks.h"

#include <libevents/libs/cpp/protocol/receive.h>
#include <libevents/libs/cpp/parse/parser.h>
#include <libevents/libs/cpp/generated/events_generated.h>

Q_DECLARE_LOGGING_CATEGORY(Events)

class EventHandler : public QObject
{
    Q_OBJECT
public:
    using send_request_event_message_f = std::function<void(const mavlink_request_event_t& msg)>;
    using handle_event_f = std::function<void(std::unique_ptr<events::parser::ParsedEvent>)>;

    EventHandler(QObject* parent, const QString& profile, handle_event_f handle_event_cb,
            send_request_event_message_f send_request_cb,
            uint8_t our_system_id, uint8_t our_component_id, uint8_t system_id, uint8_t component_id);
    ~EventHandler();


    void handleEvents(const mavlink_message_t& message);

    void setMetadata(const QString& metadataJsonFileName, const QString& translationJsonFileName);

    HealthAndArmingCheckHandler& healthAndArmingChecks() { return _health_and_arming_checks; }
private:
    void gotEvent(const mavlink_event_t& event);

    events::ReceiveProtocol* _protocol{nullptr};
    QTimer _timer;
    events::parser::Parser _parser;
    HealthAndArmingCheckHandler _health_and_arming_checks;
    QVector<mavlink_event_t> _pending_events; ///< stores incoming events until we have the metadata loaded
    handle_event_f _handle_event_cb;
    send_request_event_message_f _send_request_cb;
    const uint8_t _compid;
};