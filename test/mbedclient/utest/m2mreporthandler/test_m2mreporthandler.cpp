/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "m2mtimer_stub.h"
#include "test_m2mreporthandler.h"
#include "m2mobservationhandler.h"
#include "m2mtimer.h"
#include "m2mtimerobserver.h"
#include "m2mbase.h"

class Observer : public M2MReportObserver{

public:

    Observer() : visited(false) {}
    virtual ~Observer(){}
    virtual void observation_to_be_sent(const m2m::Vector<uint16_t>&,  uint16_t, bool){
        visited = true;
    }
    bool visited;
};

class TimerObserver : public M2MTimerObserver
{
public:
    TimerObserver() : visited(false) {}
    virtual ~TimerObserver(){}

    virtual void timer_expired(M2MTimerObserver::Type ){
        visited = true;
    }

    bool visited;
};

Test_M2MReportHandler::Test_M2MReportHandler()
{
    _observer = new Observer();
    M2MReportHandler handler(*_observer);
    _handler = new M2MReportHandler(*_observer);
}

Test_M2MReportHandler::~Test_M2MReportHandler()
{
    delete _handler;
    delete _observer;
    m2mtimer_stub::clear();
}

void Test_M2MReportHandler::test_set_under_observation()
{    
    m2mtimer_stub::enable_mock = true;

    _observer->visited = false;

    mock().clear();
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmin_timer);
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmax_timer);
    _handler->set_under_observation(true);
    CHECK(false == _observer->visited);

    mock().expectOneCall("stop_timer").onObject(&_handler->_pmin_timer);
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmax_timer);
    _observer->visited = false;    
    _handler->set_under_observation(false);
    CHECK(false == _observer->visited);
    mock().checkExpectations();

    m2mtimer_stub::enable_mock = false;
}

void Test_M2MReportHandler::test_parse_notification_attribute()
{
    char* val = {"value"};
    CHECK(false == _handler->parse_notification_attribute(val, M2MBase::ObjectInstance ));

    char* val2 = {"value&eval"};
    CHECK(false == _handler->parse_notification_attribute(val2, M2MBase::ObjectInstance ));

    char* val3 = {"toolongvaluevaluevaluevaluevalue&toolongvaluevaluevaluevaluevalue"};
    CHECK(false == _handler->parse_notification_attribute(val3, M2MBase::ObjectInstance ));

    char* val_real = {"st=6&pmax=3&lt=1&gt=100"};
    CHECK(true == _handler->parse_notification_attribute(val_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* val_real1 = {"a=1&pmin=2&pmax=3&gt=4&lt=5&st=6"};
    CHECK(false == _handler->parse_notification_attribute(val_real1, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* val2_real = {"st=6&lt=50&gt=1"};
    CHECK(false == _handler->parse_notification_attribute(val2_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* val3_real = {"gt=40&lt=5&st=6"};
    CHECK(true == _handler->parse_notification_attribute(val3_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    _handler->set_default_values();

    char* val5_real = {"pmin=10"};
    CHECK(true == _handler->parse_notification_attribute(val5_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* val6_real = {"pmin=100&pmax=5"};
    CHECK(false == _handler->parse_notification_attribute(val6_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* val7_real = {"st=6&pmax=30&lt=1&gt=100&pmin=0"};    
    CHECK(true == _handler->parse_notification_attribute(val7_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* val8_real = {"pmax=30&lt=10&gt=5&pmin=1"};
    CHECK(false == _handler->parse_notification_attribute(val8_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    // low = lt + 2 * st = 18
    char* val9_real = {"pmax=30&lt=10&gt=17&pmin=1&st=4"};
    CHECK(false == _handler->parse_notification_attribute(val9_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    // low = lt + 2 * st = 18
    char* val10_real = {"pmax=30&lt=10&gt=19&pmin=1&st=4"};
    CHECK(true == _handler->parse_notification_attribute(val10_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* val11_real = {"pmax=30&pmin=30"};
    CHECK(true == _handler->parse_notification_attribute(val11_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    _handler->set_default_values();

    char* inst_real = {"st=6&pmax=3&lt=1&gt=100"};
    CHECK(true == _handler->parse_notification_attribute(inst_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* inst1_real1 = {"a=1&pmin=2&pmax=3&gt=4&lt=5&st=6"};
    CHECK(false == _handler->parse_notification_attribute(inst1_real1, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* inst2_real = {"st=6&lt=50&gt=1"};
    CHECK(false == _handler->parse_notification_attribute(inst2_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* inst3_real = {"gt=40&lt=5&st=6"};
    CHECK(true == _handler->parse_notification_attribute(inst3_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    _handler->set_default_values();
    char* inst5_real = {"pmin=10"};
    CHECK(true == _handler->parse_notification_attribute(inst5_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* inst6_real = {"pmin=100&pmax=5"};
    CHECK(false == _handler->parse_notification_attribute(inst6_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    char* inst7_real = {"st=6&pmax=30&lt=1&gt=100&pmin=0"};
    CHECK(true == _handler->parse_notification_attribute(inst7_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* inst8_real = {"pmax=30&lt=10&gt=5&pmin=1"};
    CHECK(false == _handler->parse_notification_attribute(inst8_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    // low = lt + 2 * st = 18
    char* inst9_real = {"pmax=30&lt=10&gt=17&pmin=1&st=4"};
    CHECK(false == _handler->parse_notification_attribute(inst9_real, M2MBase::Resource,
                                                          M2MResourceInstance::INTEGER ));

    // low = lt + 2 * st = 18
    char* inst10_real = {"pmax=30&lt=10&gt=19&pmin=1&st=4"};
    CHECK(true == _handler->parse_notification_attribute(inst10_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* inst11_real = {"pmax=30&pmin=30"};
    CHECK(true == _handler->parse_notification_attribute(inst11_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    char* inst12_real = {"pmax=&pmin=30"};
    CHECK(false == _handler->parse_notification_attribute(inst12_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));
    char* inst13_real = {"st="};
    CHECK(false == _handler->parse_notification_attribute(inst13_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    _handler->set_default_values();

    char* inst14_real = {"stp=10"};
    CHECK(true == _handler->parse_notification_attribute(inst14_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));
    char* inst15_real = {"stp=10&st=15"};
    CHECK(true == _handler->parse_notification_attribute(inst15_real, M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER ));

    _handler->set_default_values();

    DOUBLES_EQUAL(0,_handler->_lt,0);
    DOUBLES_EQUAL(0,_handler->_gt,0);
    DOUBLES_EQUAL(-1,_handler->_pmax,0);
    DOUBLES_EQUAL(1,_handler->_pmin,0);
    DOUBLES_EQUAL(0,_handler->_st,0);
    DOUBLES_EQUAL(0,_handler->_high_step,0);
    DOUBLES_EQUAL(0,_handler->_low_step,0);
    DOUBLES_EQUAL(-1,_handler->_last_value,0);
    DOUBLES_EQUAL(0,_handler->_attribute_state,0);
    CHECK_FALSE(_handler->_pmin_exceeded);
    CHECK_FALSE(_handler->_pmax_exceeded);
}

void Test_M2MReportHandler::test_timer_expired()
{
    _handler->timer_expired(M2MTimerObserver::Notdefined);
    CHECK(_observer->visited == false);

    _handler->_notify = true;
    _handler->_pmin_exceeded = true;
    _handler->timer_expired(M2MTimerObserver::PMaxTimer);
    CHECK(_observer->visited == true);

    _handler->_pmin_exceeded = false;
    _handler->_notify = false;
    _handler->_attribute_state = M2MReportHandler::Pmax;
    _handler->_current_value = 100;
    _handler->timer_expired(M2MTimerObserver::PMinTimer);
    CHECK(_handler->_pmin_exceeded == true);

    _observer->visited = false;
    _handler->_notify = true;
    _handler->timer_expired(M2MTimerObserver::PMinTimer);
    CHECK(_observer->visited == true);

    _handler->_notify = true;
    _handler->_pmin_exceeded = true;
    _handler->timer_expired(M2MTimerObserver::PMinTimer);
    CHECK(_handler->_pmin_exceeded == true);

    _handler->_notify = true;
    _handler->_pmin_exceeded = false;
    _handler->_attribute_state = M2MReportHandler::Pmax;
    _handler->timer_expired(M2MTimerObserver::PMaxTimer);
    CHECK(_handler->_pmax_exceeded == true);
}

void Test_M2MReportHandler::test_set_value()
{
    _handler->_notify = true;
    _handler->_pmin_exceeded = false;
    _observer->visited = false;

    _handler->set_value(1);
    _handler->set_value(10);
    CHECK(_observer->visited == true);

    char* query = {"st=6"};
    _handler->_attribute_state = 0;
    CHECK(true == _handler->parse_notification_attribute(query,
                                                         M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER));
    _observer->visited = false;
    _handler->set_value(15);
    CHECK(_observer->visited == false);

    _observer->visited = false;
    _handler->set_value(21);
    CHECK(_observer->visited == true);

    _handler->set_value(10);
    char* query2 = {"st=3&lt=10&gt=100"};
    _handler->_attribute_state = 0;
    CHECK(true == _handler->parse_notification_attribute(query2,
                                                         M2MBase::Resource,
                                                         M2MResourceInstance::INTEGER));    

    _observer->visited = false;
    _handler->set_value(12);
    CHECK(_observer->visited == false);

    _observer->visited = false;
    _handler->set_value(15);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(5);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(4);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(101);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(102);
    CHECK(_observer->visited == true);

    char* query3 = {"lt=10"};    
    _handler->set_default_values();
    CHECK(true == _handler->parse_notification_attribute(query3, M2MBase::Resource, M2MResourceInstance::INTEGER));
    _observer->visited = false;
    _handler->set_value(9);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(15);
    CHECK(_observer->visited == false);

    char* query4 = {"gt=10"};    
    _handler->set_default_values();
    CHECK(true == _handler->parse_notification_attribute(query4, M2MBase::Resource, M2MResourceInstance::INTEGER));

    // Instantiate timers
    _handler->_attribute_state |= M2MReportHandler::Pmin;
    _handler->handle_timers();
    _observer->visited = false;
    _handler->set_value(9);
    CHECK(_observer->visited == false);    
    // Stop timers and reset flag to previous state
    _handler->stop_timers();
    _handler->_attribute_state = M2MReportHandler::Gt;

    _observer->visited = false;
    _handler->set_value(15);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    _handler->set_value(16);
    CHECK(_observer->visited == true);

    _observer->visited = false;
    char* query5 = {"gt=10"};
    _handler->set_default_values();
    CHECK(false == _handler->parse_notification_attribute(query5, M2MBase::Resource, M2MResourceInstance::STRING));

    _observer->visited = false;
    char* query6 = {"pmin=10"};
    _handler->set_default_values();
    CHECK(true == _handler->parse_notification_attribute(query6, M2MBase::Resource, M2MResourceInstance::STRING));

    _observer->visited = false;
    char* query7 = {"pmin=10&pmax=20"};
    _handler->set_default_values();
    CHECK(true == _handler->parse_notification_attribute(query7, M2MBase::Resource, M2MResourceInstance::OPAQUE));

    _observer->visited = false;
    _handler->handle_timers();
    _handler->set_value(26);
    _handler->_pmin_exceeded = true;
    _handler->timer_expired(M2MTimerObserver::PMaxTimer);
    CHECK(_observer->visited == true);
}

void Test_M2MReportHandler::test_trigger_object_notification()
{
    _handler->_notify = true;
    _handler->_pmin_exceeded = true;
    _handler->set_notification_trigger();
    CHECK(_handler->_pmin_exceeded == false);
    CHECK(_observer->visited == true);

    _handler->_changed_instance_ids.push_back(0);
    _handler->_changed_instance_ids.push_back(1);
    _handler->_changed_instance_ids.push_back(2);

    _handler->set_notification_trigger();
    CHECK(_handler->_pmin_exceeded == false);
    CHECK(_observer->visited == true);

}

void Test_M2MReportHandler::test_set_string_notification_trigger()
{
    _handler->_notify = true;
    _handler->_pmin_exceeded = true;
    _handler->set_notification_trigger();
    CHECK(_handler->_pmin_exceeded == false);
}

void Test_M2MReportHandler::test_timers()
{
    m2mtimer_stub::enable_mock = true;
    // No timers should start when _attribute_state == 0
    _handler->handle_timers();
    mock().checkExpectations();

    // pmin starts when _attribute_state |= pmin
    mock().clear();
    mock().expectOneCall("start_timer").onObject(&_handler->_pmin_timer);
    _handler->_attribute_state |= M2MReportHandler::Pmin;
    _handler->handle_timers();
    mock().checkExpectations();

    // pmin gets stopped
    mock().clear();
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmin_timer);
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmax_timer);
    _handler->stop_timers();
    mock().checkExpectations();

    // pmin starts when _attribute_state |= pmin & pmax but _pmax value is default -1
    // pmax should not start
    mock().clear();
    mock().expectOneCall("start_timer").onObject(&_handler->_pmin_timer);
    _handler->_attribute_state |= M2MReportHandler::Pmax;
    _handler->handle_timers();
    mock().checkExpectations();

    // pmin and pmax get stopped
    mock().clear();
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmin_timer);
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmax_timer);
    _handler->stop_timers();
    mock().checkExpectations();

    // max timer is started when pmax == pmin
    mock().clear();
    mock().expectOneCall("start_timer").onObject(&_handler->_pmax_timer);
    _handler->_pmax = 2;
    _handler->_pmin = 2;
    _handler->handle_timers();
    mock().checkExpectations();
    CHECK(_handler->_pmin_exceeded == true);

    /*
    mock().clear();
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmin_timer);
    mock().expectOneCall("stop_timer").onObject(&_handler->_pmax_timer);
    _handler->stop_timers();
    mock().checkExpectations();
*/
    m2mtimer_stub::enable_mock = false;
}

void Test_M2MReportHandler::test_attribute_flags()
{
    CHECK(_handler->attribute_flags() == 0);
    _handler->_attribute_state = M2MReportHandler::Pmax | M2MReportHandler::Pmin |
            M2MReportHandler::St | M2MReportHandler::Gt | M2MReportHandler::Lt | M2MReportHandler::Cancel;
    CHECK(_handler->attribute_flags() == (1 << 6) - 1);
}

void Test_M2MReportHandler::test_get_observation_token()
{
    u_int8_t test_value[] = {"val"};
    u_int32_t value_length((u_int32_t)sizeof(test_value));

    u_int8_t* out_value = (u_int8_t *)malloc(value_length);
    u_int32_t out_size = value_length;
    memcpy((u_int8_t *)out_value, (u_int8_t *)test_value, value_length);

    u_int8_t test[] = {"token"};
    _handler->_token_length = (u_int8_t)sizeof(test);
    _handler->_token = (u_int8_t *)malloc(_handler->_token_length);
    memcpy((u_int8_t *)_handler->_token, (u_int8_t *)test, _handler->_token_length);

    _handler->get_observation_token(out_value,out_size);

    CHECK(out_size == 6);

    free(out_value);
}

void Test_M2MReportHandler::test_observation_level()
{
    _handler->_observation_level = M2MBase::OR_Attribute;
    CHECK(M2MBase::OR_Attribute == _handler->observation_level());
}

void Test_M2MReportHandler::test_set_observation_token()
{
    _handler->_token_length = 4;
    _handler->_token = (u_int8_t *)malloc(_handler->_token_length);
    String test = "token";
    _handler->set_observation_token((const u_int8_t*)test.c_str(), (u_int8_t)test.size());

    CHECK(_handler->_token_length == 5);
}

void Test_M2MReportHandler::test_add_observation_level()
{
    _handler->add_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::R_Attribute == _handler->_observation_level);

    _handler->add_observation_level(M2MBase::O_Attribute);
    CHECK(M2MBase::OR_Attribute == _handler->_observation_level);
}

void Test_M2MReportHandler::test_remove_observation_level()
{
    _handler->_observation_level = M2MBase::OR_Attribute;
    _handler->remove_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::O_Attribute == _handler->_observation_level);

    _handler->remove_observation_level(M2MBase::O_Attribute);
    CHECK(M2MBase::None == _handler->_observation_level);

    _handler->_observation_level = M2MBase::OI_Attribute;
    _handler->remove_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::OI_Attribute == _handler->_observation_level);

    _handler->remove_observation_level(M2MBase::OI_Attribute);
    CHECK(M2MBase::None == _handler->_observation_level);
    _handler->remove_observation_level(M2MBase::OI_Attribute);
    CHECK(M2MBase::None == _handler->_observation_level);

}

void Test_M2MReportHandler::test_is_under_observation()
{
    CHECK(false == _handler->is_under_observation());
    _handler->_is_under_observation = true;
    CHECK(true == _handler->is_under_observation());
}
