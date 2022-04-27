# ESL_MIDTERM
TLM part:<br>
// make a directory to put all the binary files and generated Makefile<br>
mkdir build<br>
<br>
// CMake takes the current directory as build directory.<br>
cd build<br>
<br>
//Generating Makefiles, the given path must contain CMakeLists.txt<br>
cmake ..<br>
<br>
//Compile the SystemC source code.<br>
make<br>
<br>
// Run the program<br>
make run<br>
<br>
<br>
HLS part:<br>
// go to stratus directory<br>
cd stratus<br>
<br>
// The behavioral simulation (C++ only).<br>
make sim_B<br>
<br>
//The Verilog simulation for HLS config "BASIC". <br>
make sim_V_BASIC<br>
<br>
//The Verilog simulation for HLS config "DPA". <br>
make sim_V_DPA<br>
<br>
//The Verilog simulation for HLS config "UNROLL". <br>
make sim_V_UNROLL<br>
<br>
//The Verilog simulation for HLS config "PIPELINE". <br>
make sim_V_PIPELINE<br>
