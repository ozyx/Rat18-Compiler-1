namespace Rat18{
enum TransitionType
{
  REJECT = 0,
  INTEGER,
  REAL,
  IDENTIFIER,
  UNKNOWN
};

// State table
int dfsm[][5] = {{0, INTEGER, REAL, IDENTIFIER, UNKNOWN},
                 {INTEGER, INTEGER, REAL, REJECT, REJECT},
                 {REAL, REAL, REJECT, REJECT, REJECT},
                 {IDENTIFIER, REJECT, REJECT, IDENTIFIER, REJECT},
                 {UNKNOWN, REJECT, REJECT, REJECT, REJECT}};
}