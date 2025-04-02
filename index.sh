#!/bin/bash

FILE=$(mktemp)

cat > ${FILE} <<EOF
#include "index.h"

const char *index_html PROGMEM = R"rawliteral(
EOF
cat index.html >> ${FILE}

cat >> ${FILE} <<EOF
)rawliteral";

const char *index_css PROGMEM = R"rawliteral(
EOF

cat index.css >> ${FILE}

cat >> ${FILE} <<EOF
)rawliteral";
EOF

cat ${FILE}
