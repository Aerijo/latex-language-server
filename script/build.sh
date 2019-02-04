printf "Running latex-language-server build script...\n\n"

if [[ ! ${PWD##*/} == "latex-language-server" ]]; then
    printf "Must run from project root; exiting\n\n"
    exit
fi

if [ ! -d "cmake-build-debug" ]; then
    printf "making build directory \"cmake-build-debug\"\n\n"
    mkdir cmake-build-debug
fi

cd cmake-build-debug

printf "Running \"cmake ..\"\n"
cmake ..

printf "\nRunning \"cmake --build . --target latex_language_server\"\n"
cmake --build . --target latex_language_server

printf "\nFinished build script\n"
