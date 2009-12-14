#! /bin/sh

class=$1
cap=$(echo "$1" | tr a-z A-Z)

add() {

  svn revert $1 > /dev/null 2>&1
  git add $1
  svn propset svn:keywords 'Id' $1
}

sed -e "s/@FILECAP@/$cap/g" \
    -e "s/@FILE@/$class/g" \
    misc/template.cpp \
    > src/$1.cpp

sed -e "s/@FILECAP@/$cap/g" \
    -e "s/@FILE@/$class/g" \
    misc/template.h \
    > src/$1.h

sed -e "s/@FILECAP@/$cap/g" \
    -e "s/@FILE@/$class/g" \
    misc/templateTest.cpp \
    > src/tests/${1}Test.cpp

sed -e "s/@FILECAP@/$cap/g" \
    -e "s/@FILE@/$class/g" \
    misc/templateTest.h \
    > src/tests/${1}Test.h

add src/$1.cpp
add src/$1.h
add src/tests/${1}Test.cpp
add src/tests/${1}Test.h

