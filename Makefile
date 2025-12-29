


clean:
	rm -rf build/

format:
	find src include -type f \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 clang-format -i
