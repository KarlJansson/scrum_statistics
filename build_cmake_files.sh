cmakemaker

rm -rf ./_build_release_
mkdir _build_release_
cd _build_release_
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_USER_MAKE_RULES_OVERRIDE=~/ClangOverrides.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
cp compile_commands.json ..
