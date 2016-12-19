FINALEXAM README 

Joe,

Here are some things that I'd like to point out for when you are grading, sorry if it's lengthy!

Currently, this game engine / solution(s) start out with the operator new, new[], delete, and delete[]
overrides COMMENTED OUT. This is so that you can properly test the various parts of the Memory Management
system without having to worry about weirdness with the use of Vectors in the various Unit Tests. You can
find the tests that I use in "MonsterTesting.cpp".

NOTE: LEAVE THE VARIOUS OPERATOR OVERRIDES COMMENTED OUT UNTIL POINTED OUT BELOW

TESTING
	TO RUN BLOCK ALLOCATOR TESTS:
	Uncomment out the appropriate define flag in "Main.cpp" entitled BLOCK_ALLOCATOR_TESTS. This will run your unit
	tests with and without alignment support. NOTE: I actually rewrote my block alloactor prior to this final exam
	to make sure that it properly supports alignment :) You can find the implementation of this block allocator
	in "GAllocator.cpp" and the correseponding .h file.

	TO RUN BIT ARRAY TESTS:
	Uncomment out the appropriate define flag in "Main.cpp" entitled FSA_TESTS to run your unit tests against my BitArray
	implementations. More on the exact implementation later but it runs it against a bit array size of 1 to 1000.

	TO RUN FIXED SIZED ALLOCATOR TESTS:
	Just like above, there's a convenient FSA_TESTS define in "Main.cpp" that you can uncomment out to run these tests.
	It tests my FSA implementation in block size of 128 bytes (You can change this on line 373 of MonsterTesting.cpp)
	on numbers of blocks from 1 to 1000. I didn't go past 128 often as it yielded weird results. (read, broken).

	TO RUN GAME LOOP USING STL CONTAINERS:
	Uncomment the various operator overrides, NOW. Making sure that the various flags for testing are all commented out. You 
	only want to run the game loop only.
ENDTESTING

NOTES
	"MemoryManager.cpp" and the corresponding .h file contain the MemoryManagement system. Within the constructor on lines 8-9
	you can see that it sets up itself, the FSAs, and their corresponding BitArrays through the pass through of a single reference 
	to the GAllocator singleton. So the block allocator in itself, sets up the memory sytem. As well as being a part of it.

	Concerning 32/64 bit support. You will see the use of size_t and size_t* throughout various systems like FSA and BlockAllocator. 
	The one place that I'd like to point out that I deviate from the norm is in BitArray.h on lines 12 - 19 where I utilize a uint32_t
	and uint64_t wrapped in a typedef to do my pointer arithmetic for manipulation throughout my BitArray. Sure, size_t would have been
	sufficient, but I wanted to be SPECIFICALLY CLEAR to the programmer what I'm using the type for. size_t to me doesn't immediately
	read as prevalent for the 4 to 8 byte conversion math than something like uint32_t. I also use a #define in the private member
	variables of the BitArray, concerning the bit check that I use to set and clear bits on lines 51-55 of BitArray.h. Finally, a 
	macro definition for BITSCAN is set at the top of that .h file.

	I intended to use the _BITSCANFORWARD() and _BITSCANFORWARD64() functions to their potential in my BitArray manipulation, you
	can see that with my use of binary negation and "container checks" in the GetFirstSet() and GetFirstClear() functions.

	In the various ~Destructors() of my FixedSizeAllocators, GAllocator / BlockAllocator there are debug checks for outsanding
	allocations.

	Finally, in terms of styling. I used the i_ and o_ parameters along with camel case for function inputs and outputs. Using 
	pass by reference when referring to actual types and not pointers or primitives. Member variables are underline separated with
	a trailer underline to be specific.

	You can find some ternary operators on lines 73 of FixedSizeAllocator.cpp, and 85/99 of BitArray.cpp. Electing to use them
	when it was simplest to read/understand.


ENDNOTES

	In conclusion, I found that parts of my systems worked most cleanly when tested separately via unit tests, but there may be 
	leaks here or there when combined, especially with the operator overrides. Something that I'd like to fix before the next
	semester begins. 

	Thank you for the extra time and for the great semester!

	You can reach me on the eae-mgs slack via @garinrk if you have any questions or concerns and I'll make sure to get back
	to you asap.


Thanks,
Garin