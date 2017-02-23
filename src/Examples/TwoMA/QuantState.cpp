#include "stdafx.h"
//
#include "QuantState.h"

std::vector<QuantState *> g_qs;

QuantState &QuantState::operator=(const QuantState &x) {
	if (this != &x) {
		memcpy(this,&x,sizeof(QuantState));
	}
	return *this;
}

QuantState::QuantState(const QuantState &x)
{
	memcpy(this,&x,sizeof(QuantState));
}

QuantState::QuantState()
{
	// WARNING, this only works on a SIMPLE structure. No virtual anything. 
	memset(this,0,sizeof(QuantState));
}

QuantState *NewQuantState() {
  QuantState *pl = new QuantState();
  g_qs.push_back(pl);
  return pl;
}

void FreeQuantState(QuantState *pl) {
  if (!pl) {
    return;
  }

  int n = (int)g_qs.size();
  for (int i = 0; i < n; i++) {
    if (pl == g_qs[i]) {
      g_qs.erase(g_qs.begin() + n);
      break;
    }
  }
  delete pl;
}
