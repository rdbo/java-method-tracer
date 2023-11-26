all:
	c++ -o libtest.so -shared -fPIC main.cpp
	javac target/main/Main.java
