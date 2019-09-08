#include "base/StateChartBase.hpp"
void IStateChartBase::StateTransition(IStateBase* next_state) {
    MLOG(logINFO) << (current_state_? current_state_->Name(): "InitState") << " ==> " << next_state->Name();
    next_state_ = next_state;
}