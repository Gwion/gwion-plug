#!/bin/sh
name=${1,,}

cat << EOF > Fx/"${name}".cpp
#include <cv.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "${name}.hpp"

void ${1}::run(Loup* loup, Mat mat) {


}
EOF

cat << EOF > include/"${name}".hpp
class ${1} : public Fx {
  void run(Loup*, Mat);
};
EOF

echo   "please add:"
echo   "#include \"${name}.hpp"\"
echo   "to the header section of plug.cpp"
echo   "please add:"
printf "\telse if(str == \"%s\")\n" "${name}"
printf "\t\tret = LOUP(o)->add(new %s);\n" "${1}"
echo   "to the run() section of plug.cpp"
