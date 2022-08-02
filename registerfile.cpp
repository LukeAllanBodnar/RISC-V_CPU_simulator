#include "registerfile.h"
#include "memory.h"
#include "rv32i_decode.h"

using namespace std;

void registerfile::reset() {

	regs.resize(32, 0xf0f0f0f0);
	regs.at(0) = 0;

}

void registerfile::set(uint32_t r, int32_t val) {

	if (r != 0) {
		regs.at(r) = val;
	}

}

int32_t registerfile::get(uint32_t r) const {

	return regs.at(r);

}

void registerfile::dump(const string& hdr) const {

	
	

	for (uint32_t p = 0; p < 32 ; ++p) {

		if (p % 8 == 0) {

			if (p != 0) {
				cout << endl;

			}

			cout << hdr;

			std::ostringstream os;
			os << "x" << p;
			cout << setw(3) << right << os.str();

		}

		if (p % 8 != 0 && p % 4 == 0) {
			cout << " ";
		}

		cout << " " << hex::to_hex32(regs.at(p));

	}

}
