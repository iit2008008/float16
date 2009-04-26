/*
 * DFS Assignment - A new Float Datatype
 *
 * Specifications:-
 *
 *	Name: float16
 * 	Size: 16 bytes
 * 	Allocation: 
 * 		Byte 1: 	Sign of Mantissa
 * 		Byte 2-32: 	Mantissa
 * 		Byte 33: 	Sign of Exponent
 * 		Byte 34-128: 	Exponent
 *
 * 	e.g.- The number 123.45e30 is stored as - 
 * 		Mantissa = 12345
 * 		Exponent = 32
 *
 * Team Members: 
 *
 * iit2008001 - Malay Singh
 * iit2008002 - Anupam Srivastava
 * iit2008003 - Yagyadutt Mishra
 * iit2008004 - Shubham Gupta
 * iit2008005 - Anvay Srivastava
 * iit2008006 - Shailendra Upadhyaya
 * iit2008007 - Shubham Aggarwal
 * iit2008008 - Pranjal Kumar Singh
 * iit2008009 - Santak Dalai
 * iit2008010 - Sunny Shulka
 *
 */
#include <iostream>
#include <cassert>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <bitset>

using namespace std;

class float16 {
	private:
		unsigned char num[16];
		string input;
	public:
		int mantissa;
		string exponent;
		bool sm,se;

		float16();

		//------Locating Functions------
		int atm(int); //locate a mantissa bit
		int ate(int); //locate an exponent bit
		int at(int,int); //locate any bit of a row
		//------------------------------

		//------Storage Functions-------
		void store(string);
		void disseminate(string);
		string toHex(string);
		void saveMantissa(int);
		void saveExponent(string);
		//------------------------------
		
		//String addition functions
		string addToString(string,int);
		string addStrings(string,string);
		string subStrings(string,string);

		//Display Functions
		void bitDisplay();
		string returnMantissa();
		string returnExponent();

		//Operator Overloading

		friend istream & operator >> (istream &, float16 & );
		friend ostream & operator << (ostream &, float16 & );
};

//------------------Arithmetic Function Prototypes--------------------------
float16 addFloats(float16,float16);
//Subtraction is a special case of addition only... so no separate function, just an operator
float16 mulFloats(float16 a, float16 b);
float16 divFloats(float16 a, float16 b);
bool isGreater(string,string);
bitset<30> addBitsets(bitset<30> m1, bitset<30> m2);
//-------------------------------------------------------------------------
bool isGreater(string a, string b)
{
	while(a.size() < b.size()) a.insert(0,"0");
	while(b.size() > a.size()) b.insert(0,"0");
	for(int i=0; i<a.size(); i++) {
		if(a[i] > b[i]) {
			return true;
		}
	}
	return false;
}
//Functions for Division------------------------------------
bitset<30> sub(bitset<30>m1,bitset<30>m2)
{
	//------For subtraction------
	if(m1==m2){
		bitset<30>c1;
		return c1;
	}

	m2=~m2;
	bitset<30>c1,s1;
	for(int i=1;i<c1.size()-2;i++){
		s1[i-1]=c1[i-1]^m1[i-1]^m2[i-1];
		c1[i]=(m1[i-1]&m2[i-1])|((m1[i-1]^m2[i-1])&c1[i-1]);
	}

	long s=s1.to_ulong()+1;
	bitset<30>w(s);

	return w;

}

int size1(bitset<30> m1){
	
	for(int i=m1.size()-1;i>=0;i--){
			if(m1[i]==1){
				return i;
			}
	}
}

long long int fractional(long long int man1,long long int man2)
{
	//------For calculation of fractional part------
	bitset<30>ma1(man1);
	bitset<30>ma2(man2);
	bitset<30>temp,q,rem;

	int i=ma1.size()-1;
	int pos;

	while(ma1.to_ulong()>=ma2.to_ulong() ){
	
		if(ma1[i]==1){
			temp[0]=1;
		}

		if(temp.to_ulong()<ma2.to_ulong() ){
			temp=temp<<1;
			q=q<<1;
		} else{
			q=q<<1;
			q[0]=1;
			rem=sub(temp,ma2);
			temp.reset();
			for(int k=0;k<rem.size();k++){
				temp[k]=rem[k];
			}
			temp=temp<<1;
			rem=rem<<i;

			for(int j=0;j<i;j++){
				rem[j]=ma1[j];
			}

			for(int k=0;k<ma1.size();k++){
				ma1[k]=rem[k];
			}
			pos=i;

			rem.reset();
		}
		i--;
	}
	q=q<<pos;

	return q.to_ulong();
}
 
long long int decimal(long long int ma1,long long int ma2)
{
	//------For calculation of decimal part------
	bitset<30>de1(ma1);
	bitset<30>de2(ma2);
	bitset<30>fr,reman;
	de1=de1<<1;

	for(int lim=0;lim<9;lim++){
		if( de1.to_ulong()>=de2.to_ulong() ){
			fr[0]=1;
			reman=sub(de1,de2);
			for(int k=0;k<de1.size();k++){
				de1[k]=reman[k];
			}
		} 
		fr=fr<<1;
		de1=de1<<1;
	}
	long long int total=0;
	for(int i=fr.size()-1;i>=0;i--){
		total=total+ fr[i]/pow(2,(fr.size()-i))*(pow(10,10));
	}

	return total;
}
float16 divFloats(float16 ffa, float16 ffb)
{
	float16 result;
	bool sign = false;
	long long int ma1, ma2;
	stringstream ss;
	ss << ffa.returnMantissa();
	ss >> ma1;
	ss.clear();
	ss.str("");

	ss << ffb.returnMantissa();
	ss >> ma2;
	ss.clear();
	ss.str();

	if(ma1 < 0 && ma2 > 0) {
		ma1 *= -1;
		sign = true;
	} else if(ma1 > 0 && ma2 < 0) {
		ma2 *= -1;
		sign = true;
	}

	string e1, e2;
	e1 = ffa.returnExponent();
	e2 = ffb.returnExponent();

	string fin;
	long long int frac,dec,total;
	frac=fractional(ma1,ma2);

	dec=decimal( (ma1-frac*ma2),ma2);

	total = (frac*pow(10,10)+dec);
	int decc = 0;
	while(total%10 == 0) {
		total/=10;
		decc++;
	}
	decc -= 10;
	if(sign) total *= -1;
	string exp;
	if(e1[0] != '-' && e2[0] != '-') {
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		if(isGreater(e1,e2)) {
			exp = result.subStrings(e1,e2);
		} else {
			exp = result.subStrings(e2,e1);
			while(exp[0] == '0') exp.erase(exp.begin());
			if(exp != "") exp.insert(0,"-");
		}
	} else if(e1[0] == '-' && e2[0] != '-') {
		e1.erase(e1.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		exp = result.addStrings(e2,e1);
		while(exp[0] == '0') exp.erase(exp.begin());
		if(exp != "") exp.insert(0,"-");
	} else if(e1[0] != '-' && e2[0] == '-') {
		e2.erase(e2.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		exp = result.addStrings(e1,e2);
	} else if(e1[0] == '-' && e2[0] == '-') {
		e1.erase(e1.begin());
		e2.erase(e2.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		if(isGreater(e2,e1)) {
			exp = result.subStrings(e2,e1);
		} else {
			exp = result.subStrings(e1,e2);
			while(exp[0] == '0') exp.erase(exp.begin());
			if(exp != "") exp.insert(0, "-");
		}
	}
	exp = result.addToString(exp,decc);
	result.saveMantissa(total);
	result.saveExponent(exp);
	return result;
}
//-------------------------------------------------------------------
float16 mulFloats(float16 ffa, float16 ffb)
{
	float16 result;
	string m1, m2, e1, e2;
	bool sm=false;
	m1 = ffa.returnMantissa();
	m2 = ffb.returnMantissa();
	e1 = ffa.returnExponent();
	e2 = ffb.returnExponent();
	
	if((m1[0] == '-' && m2[0] != '-')||(m1[0] != '-' && m2[0] == '-')) {
		sm = true;
	}
	long int num;
	string exp;
	if(e1[0] != '-' && e2[0] != '-') {
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		exp = result.addStrings(e1,e2);
	} else if(e1[0] == '-' && e2[0] != '-') {
		e1.erase(e1.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		if(isGreater(e2,e1)) {
			exp = result.subStrings(e2,e1);
		} else {
			exp = result.subStrings(e1,e2);
			while(exp[0] == '0') exp.erase(exp.begin());
			if(exp != "") exp.insert(0,"-");
		}
	} else if(e1[0] != '-' && e2[0] == '-') {
		e2.erase(e2.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		if(isGreater(e1,e2)) {
			exp = result.subStrings(e1,e2);
		} else {
			exp = result.subStrings(e2,e1);
			while(exp[0] == '0') exp.erase(exp.begin());
			if(exp != "") exp.insert(0,"-");
		}
	} else if(e1[0] == '-' && e2[0] == '-') {
		e1.erase(e1.begin());
		e2.erase(e2.begin());
		while(e1.size()<e2.size()) e1.insert(0,"0");
		while(e2.size()<e1.size()) e2.insert(0,"0");
		exp = result.addStrings(e1,e2);
		exp.insert(0, "-");
	}
	long int num1;
	stringstream ss;
	ss << m1;
	ss >> num;
	ss.str("");
	ss.clear();
	ss << m2;
	ss >> num1;
	if(num1 < 0) num1 *= -1;
	if(num < 0) num *= -1;
	vector< bitset<30> > vb;
	bitset <30>b(num);
	bitset <30>b1 = b;
	bitset <30>temp = b;
	int n = 0;
	int i = 0;
	while(num1 != 0 && num1 != 1){
		while(pow(2,n) <= num1){
			n++;
		}
		int i;
		for(i = 29; i >= 0; i--)
		{
			if (b1[i]==1)
				break;
		}
		for(int j = i; j >= 0; j--){
			if ((j+n-1)>29) {
				bitset <30> cy(long(  b1.to_ulong()/10));
				b1=cy;
				exp = result.addToString(exp, 1);
				continue;
			}	
			b1[j+n-1] = b1[j];
			b1[j]=0;
		}
		num1 = num1 - int(pow(2,n-1));
		vb.push_back(b1);
		i++;	
		b1 = temp;
		n = 0;
	}

	if ( num1 == 1 ) {
		vb.push_back(b);
	}	
	bitset<30> add(0);
	add = vb[0];
	for(int k = 1; k < vb.size(); k++){
		add = addBitsets(add,vb[k]);	
	}	
	long man = add.to_ulong();
	if(sm) man *= -1;
	result.saveMantissa(man);
	result.saveExponent(exp);
	
	return result;
}

bitset<30> addBitsets(bitset<30> m1, bitset<30> m2)
{
	vector<int> v1,v2;
	bitset<96> e1(0);
	bitset<96> e2(0);
	bitset<30> c1,s4,c4;
	int p1=0;
	int p2=0;
	int m11=0;
	int m22=0;
	long int x;
	int flag=0;
	bitset<30> s1,s2;
	bitset<96> se1,se2;

	x=0;
	for(int i=m1.size()-1;i>-1;i++){
		s1[i]=m1[i]&1;
		if(s1[i]==1)
			break;
		else
			m11++;
	}
	for(int i=m2.size()-1;i>-1;i++){
		s2[i]=m2[i]&1;
		if(s2[i]==1)
			break;
		else
			m22++;
	}	
	for(int i=95;i>-1;i--){
		se1[i]=e1[i]&1;
		se2[i]=e2[i]&1;
		if(se1[i]==1){
			v1.push_back(p1);
			p1++;
		}else
			p1++;
		if(se2[i]==1){
			v2.push_back(p2);
			p2++;

		}else
			p2++;
			
	}
	int i=0;
	int a=v1.size();
	int b=v2.size();
	int temp;
	long int y;
	bitset<97> c;
	bitset<96> s;	
	bitset<96> s3;
	if(a>=b){
		for(int i=0;i<a-b;i++)
			v2.push_back(100);

	}else{
		for(int i=0;i<b-a;i++)
			v1.push_back(100);
	}
	int b1;	
	while(i!=v1.size()){
		if(v1[i]<v2[i]){
			e2=~e2;
			b1=i;
			flag=1;
			break;
		}else if(v1[i]>v2[i]){
			e1=~e1;
			b1=i;
			flag=1;
			break;
		}
		i++;
	}
	if(flag==1){
		for(int j=1;j<c.size();j++){
			s[j-1]=c[j-1]^e1[j-1]^e2[j-1];
			c[j]=(e1[j-1]&e2[j-1])|((e1[j-1]^e2[j-1])&c[j-1]);
		}
		long int val1;
		val1=s.to_ulong()+1;
		bitset<96> su=long(val1);
		for(int j=su.size()-1;j>-1;j--){
			s3[j]=su[j]&1;
		}
		int a1=0;
		long int z;
		for(int j=s3.size()-1;j>-1;j--){
			if(s3[j]==1){
				break;
			}
			a1++;
		}
		a1=95-a1;
		if(a1>=4){
			if(v1[b1]<v2[b1]){
				return m1;
			}else if(v1[b1]>v2[b1]){
				return m2;
			}
		}else if(a1<4){
			z=su.to_ulong();
			z=pow(10,z);
			if(v1[b1]<v2[b1]){
				x=pow(2,31-m11);
				if(x>=z){
					long int val;
					val=m1.to_ulong();
					val=val*z;
					bitset<30> m3=(long(val));
					bitset<30> s41;
					bitset<30> c41;
					for(int j=1;j<c41.size();j++){
						s41[j-1]=c41[j-1]^m3[j-1]^m2[j-1];
						c41[j]=(m3[j-1]&m2[j-1])|((m3[j-1]^m2[j-1])&c41[j-1]);
					}
					return s41;

				}else if(x<z){
					return m1;
				}
			}else if(v1[b1]>v2[b1]){
				x=pow(2,31-m22);
				if(x>=z){
					long int val;
					val=m2.to_ulong();
					val=val*z;
					bitset<30> m3=(long(val));
					bitset<30> s41;
					bitset<30> c41;
					for(int j=1;j<c41.size();j++){
						s41[j-1]=c41[j-1]^m1[j-1]^m3[j-1];
						c41[j]=(m1[j-1]&m3[j-1])|((m1[j-1]^m3[j-1])&c41[j-1]);
					}
					return s41;

				}else if(x<z){
					return m2;
				}
			}
		}
	} else {
		bitset<30> s41;
		bitset<33> c41;
		for(int j=1;j<c41.size();j++){
			s41[j-1]=c41[j-1]^m1[j-1]^m2[j-1];
			c41[j]=(m1[j-1]&m2[j-1])|((m1[j-1]^m2[j-1])&c41[j-1]);
		}
		return s41;
	}
}
float16 addFloats(float16 fa, float16 fb)
{
	float16 fc;

	vector<int> v1,v2,v3;
	bitset<32> m1(0);
	bitset<32> m2(0);
	bitset<96> e1(0);
	bitset<96> e2(0);
	int sg1=0;
	bitset<32> c1,s4,c4;
	int p1=0;
	int p2=0;
	int m11=0;
	int m22=0;
	int fin=0;
	long int x,e,z1,m;
	int flag=0;
	bitset<32> s1,s2;
	bitset<96> se1,se2;
	x=0;
	for(int i=0; i<31; i++) {
		m1[i] = fa.atm(i);
		m2[i] = fb.atm(i);
	}
	for(int i=0;i<31;i++){
		if(m1[i]!=m2[i]){
			fin=1;
			break;
		}
	}
	for(int i=0; i<95; i++) {
		e1[i] = fa.ate(i);
		e2[i] = fb.ate(i);
	}
	for(int i=0;i<95;i++){
		if(e1[i]!=e2[i]){
			fin=1;
			break;
		}
	}
	if(fa.ate(95)!=fb.ate(95)){
		fin=1;
	}
	for(int i=m1.size()-1;i>-1;i++){
		s1[i]=m1[i]&1;
		if(s1[i]==1)
			break;
		else
			m11++;
	}
	for(int i=m2.size()-1;i>-1;i++){
		s2[i]=m2[i]&1;
		if(s2[i]==1)
			break;
		else
			m22++;
	}
	if(fa.atm(31)==1 && fb.atm(31)==0){
		if(fin==0){
			fc.saveMantissa(0);
			fc.saveExponent("0");
			return fc;
		}
		m1=~m1;
	}else if(fa.atm(31)==0 && fb.atm(31)==1){
		if(fin==0){
			fc.saveMantissa(0);
			fc.saveExponent("0");
			return fc;
		}
		m2=~m2;
	}
	if(fa.atm(31)==1 && fb.atm(31)==1){
		v3.push_back(1);
	}else if(fa.atm(31)==0 && fb.atm(31)==0){
		v3.push_back(0);
	}
	if(fa.ate(95)==1 && fb.ate(95)==1){
		v3.push_back(1);
	}else if(fa.ate(95)==0 && fb.ate(95)==0){
		v3.push_back(0);
	}else if(fa.ate(95)==1 && fb.ate(95)==0){
		v3.push_back(1);
		sg1=1;
	}else if(fa.ate(95)==0 && fb.ate(95)==1){
		sg1=2;
		v3.push_back(1);
	}
	bitset<96> em,em1;
	bitset<97> ec;
	int fg=0;
	if(fa.ate(95)!=fb.ate(95)){
		fg=1;
	}else{
		for(int i=0;i<e1.size()-1;i++){
			if(e1[i]!=e2[i]){
				fg=1;
				break;
			}
		}
	}
	
	if(fg==1){	
		for(int j=1;j<ec.size();j++){
			em[j-1]=ec[j-1]^e1[j-1]^e2[j-1];
			ec[j]=(e1[j-1]&e2[j-1])|((e1[j-1]^e2[j-1])&ec[j-1]);
		}
		int e11=0;
		for(int i=em.size()-1;i>-1;i--){
			em1[i]=em[i]&1;
			if(em1[i]==1){
				break;
			}
			e11++;
		}
		e11=95-e11;
		if(e11>=4){
			if(sg1==1){
				if(v3[0] == 1) fb.mantissa *= -1;
				if(v3[1] == 1) fb.exponent.insert(0,"-");
				fc.saveMantissa(fb.mantissa);
				fc.saveExponent(fb.exponent);
				return fc;
			}else if(sg1==2){
				if(v3[0] == 1) fa.mantissa *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(fa.mantissa);
				fc.saveExponent(fa.exponent);
				return fc;
			}
		}else if(e11<4){
			if(sg1==1){
				e=pow(2,31-m22);
				z1=em.to_ulong();
				z1=pow(10,z1);
				if(e<z1){		
					if(v3[0] == 1) fb.mantissa *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
					fc.saveMantissa(fb.mantissa);
					fc.saveExponent(fb.exponent);
					return fc;
				}else if(e>z1){
					m=m1.to_ulong();
					m=m*z1;
					bitset<32> m111=(long(m));
					bitset<32> sm;
					bitset<33> cm;
					for(int j=1;j<m111.size();j++){
						sm[j-1]=cm[j-1]^m111[j-1]^m2[j-1];
						cm[j]=(m111[j-1]&m2[j-1])|((m111[j-1]^m2[j-1])&cm[j-1]);
					}
					long int mant = sm.to_ulong();
					if(v3[0] == 1) mant *= -1;
					if(v3[1] == 1) fa.exponent.insert(0,"-");
					fc.saveMantissa(mant);
					fc.saveExponent(fa.exponent);
					return fc;
				}
			}else if(sg1==2){
				e=pow(2,31-m11);
				z1=em.to_ulong();
				z1=pow(10,z1);
				if(e<z1){
					if(v3[0] == 1) fa.mantissa *= -1;
					if(v3[1] == 1) fa.exponent.insert(0,"-");
					fc.saveMantissa(fa.mantissa);
					fc.saveExponent(fa.exponent);
					return fc;
				}else if(e>z1){
					m=m2.to_ulong();
					m=m*z1;
					bitset<32> m222=(long(m));
					bitset<32> sm;
					bitset<33> cm;
					for(int j=1;j<m222.size();j++){
						sm[j-1]=cm[j-1]^m222[j-1]^m1[j-1];
						cm[j]=(m222[j-1]&m1[j-1])|((m222[j-1]^m1[j-1])&cm[j-1]);
					}
					long int mant = sm.to_ulong();
					if(v3[0] == 1) mant *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
					fc.saveMantissa(mant);
					fc.saveExponent(fb.exponent);
					return fc;
				}
			}
		}
	}else if(fg==0){
		bitset<32> fs;
		bitset<33> fcc;
		for(int j=1;j<m1.size();j++){
			fs[j-1]=fcc[j-1]^m1[j-1]^m2[j-1];
			fcc[j]=(m1[j-1]&m2[j-1])|((m1[j-1]^m2[j-1])&fcc[j-1]);
		}
		
		long int mant = fs.to_ulong();
		if(v3[0] == 1) mant *= -1;
		if(v3[1] == 1) fa.exponent.insert(0,"-");
		fc.saveMantissa(mant);
		fc.saveExponent(fa.exponent);
		return fc;
	}


		
	for(int i=95;i>-1;i--){
		se1[i]=e1[i]&1;
		se2[i]=e2[i]&1;
		if(se1[i]==1){
			v1.push_back(p1);
			p1++;
		}else
			p1++;
		if(se2[i]==1){
			v2.push_back(p2);
			p2++;

		}else
			p2++;
			
	}
	int i=0;
	int a=v1.size();
	int b=v2.size();
	int temp;
	long int y;
	bitset<97> c;
	bitset<96> s;	
	bitset<96> s3;
	if(a>=b){
		for(int i=0;i<a-b;i++)
			v2.push_back(100);

	}else{
		for(int i=0;i<b-a;i++)
			v1.push_back(100);
	}
	int b1;	
	while(i!=v1.size()){
		if(v1[i]<v2[i]){
			e2=~e2;
			b1=i;
			flag=1;
			break;
		}else if(v1[i]>v2[i]){
			e1=~e1;
			b1=i;
			flag=1;
			break;
		}
		i++;
	}
	if(flag==1){
		for(int j=1;j<c.size();j++){
			s[j-1]=c[j-1]^e1[j-1]^e2[j-1];
			c[j]=(e1[j-1]&e2[j-1])|((e1[j-1]^e2[j-1])&c[j-1]);
		}
		long int val1;
		val1=s.to_ulong()+1;
		bitset<96> su=long(val1);
	for(int j=su.size()-1;j>-1;j--){
		s3[j]=su[j]&1;
	}
	int a1=0;
	long int z;
	for(int j=s3.size()-1;j>-1;j--){
		if(s3[j]==1){
			break;
		}
		a1++;
	}
	a1=95-a1;
	if(a1>=4){
		if(v1[b1]<v2[b1]){
			if(fa.ate(95)==1 && fb.ate(95)==1){
				if(v3[0] == 1) fb.mantissa *= -1;
				if(v3[1] == 1) fb.exponent.insert(0,"-");
				fc.saveMantissa(fb.mantissa);
				fc.saveExponent(fb.exponent);
			}else{
				if(v3[0] == 1) fa.mantissa *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(fa.mantissa);
				fc.saveExponent(fa.exponent);
			}
			return fc;
		}else if(v1[b1]>v2[b1]){
			if(fa.ate(95)==1 && fb.ate(95)==1){
					if(v3[0] == 1) fa.mantissa *= -1;
					if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(fa.mantissa);
				fc.saveExponent(fa.exponent);
			}else{
					if(v3[0] == 1) fb.mantissa *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
			fc.saveMantissa(fb.mantissa);
			fc.saveExponent(fb.exponent);
			}
			return fc;
		}
	}else if(a1<4){
		z=su.to_ulong();
		z=pow(10,z);
		if(v1[b1]<v2[b1]){
			if(fb.ate(95)==1 && fa.ate(95)==1){
				x=pow(2,31-m22);
			}else{
				x=pow(2,31-m11);
			}
			if(x>=z){
				long int val;
				if(fb.ate(95)==1 && fa.ate(95)==1){
					val=m2.to_ulong();
				}else{
				val=m1.to_ulong();
				}
				val=val*z;
				bitset<32> m3=(long(val));
				bitset<32> s41;
				bitset<32> c41;
		    if(fa.ate(95)==1 && fb.ate(95)==1){
			for(int j=1;j<c41.size();j++){
			s41[j-1]=c41[j-1]^m3[j-1]^m1[j-1];
			c41[j]=(m3[j-1]&m1[j-1])|((m3[j-1]^m1[j-1])&c41[j-1]);
		      }
				long int mant = s41.to_ulong();
				if(v3[0] == 1) mant *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(mant);
				fc.saveExponent(fa.exponent);
		}else{
			for(int j=1;j<c41.size();j++){
			s41[j-1]=c41[j-1]^m3[j-1]^m2[j-1];
			c41[j]=(m3[j-1]&m2[j-1])|((m3[j-1]^m2[j-1])&c41[j-1]);
			}
				long int mant = s41.to_ulong();
				if(v3[0] == 1) mant *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(mant);
				fc.saveExponent(fb.exponent);
		}

			}else if(x<z){
				if(fa.ate(95)==1 && fb.ate(95)==1){

					if(v3[0] == 1) fb.mantissa *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
					fc.saveMantissa(fb.mantissa);
					fc.saveExponent(fb.exponent);
					return fc;
			}else{
					if(v3[0] == 1) fa.mantissa *= -1;
					if(v3[1] == 1) fa.exponent.insert(0,"-");
				fc.saveMantissa(fa.mantissa);
				fc.saveExponent(fa.exponent);
				return fc;
			}
	
		}
	}else if(v1[b1]>v2[b1]){
		if(fa.ate(95)==1 && fb.ate(95)==1){
			x=pow(2,31-m11);
		}else{
				
			x=pow(2,31-m22);
		}
		if(x>=z){
			long int val;
				if(fa.ate(95)==1 && fb.ate(95)==1){
				val=m1.to_ulong();
				}else{
					val=m2.to_ulong();
				}
				val=val*z;
				bitset<32> m3=(long(val));
				bitset<32> s41;
				bitset<33> c41;
			if(fa.ate(95)==1 && fb.ate(95)==1){
				for(int j=1;j<c41.size();j++){
				s41[j-1]=c41[j-1]^m2[j-1]^m3[j-1];
				c41[j]=(m2[j-1]&m3[j-1])|((m2[j-1]^m3[j-1])&c41[j-1]);
				}
				long int mant = s41.to_ulong();
				if(v3[0] == 1) mant *= -1;
				if(v3[1] == 1) fb.exponent.insert(0,"-");
				fc.saveMantissa(mant);
				fc.saveExponent(fb.exponent);
			}else{
			for(int j=1;j<c41.size();j++){
			s41[j-1]=c41[j-1]^m1[j-1]^m3[j-1];
			c41[j]=(m1[j-1]&m3[j-1])|((m1[j-1]^m3[j-1])&c41[j-1]);
		}
				long int mant = s41.to_ulong();
				if(v3[0] == 1) mant *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
			fc.saveMantissa(mant);
			fc.saveExponent(fa.exponent);
			}
			}else if(x<z){
				if(fa.ate(95)==1 && fb.ate(95)==1){
					if(v3[0] == 1) fa.mantissa *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
					fc.saveMantissa(fa.mantissa);
					fc.saveExponent(fb.exponent);
				}else{
					if(v3[0] == 1) fb.mantissa *= -1;
					if(v3[1] == 1) fb.exponent.insert(0,"-");
				fc.saveMantissa(fb.mantissa);
				fc.saveExponent(fb.exponent);
				}
				return fc;
			}
		}
	
	} else if(flag==0){
		bitset<32> s41;
		bitset<33> c41;
		for(int j=1;j<c41.size();j++){
			s41[j-1]=c41[j-1]^m1[j-1]^m2[j-1];
			c41[j]=(m1[j-1]&m2[j-1])|((m1[j-1]^m2[j-1])&c41[j-1]);
		}
				long int mant = s41.to_ulong();
				if(v3[0] == 1) mant *= -1;
				if(v3[1] == 1) fa.exponent.insert(0,"-");
		fc.saveMantissa(mant);
		fc.saveExponent(fa.exponent);
	}
	}
	return fc;
}
//------------------Operator Overloading------------------------------------
float16 operator + (float16 m1,float16 m2)
{
	return addFloats(m1,m2);
}

float16 operator - (float16 m1,float16 m2)
{
	float16 ff;
	string s = m2.returnMantissa();
	if(s[0] == '-') s.erase(s.begin());
	else s.insert(0,"-");
	stringstream ss;
	long int m;
	ss << s;
	ss >> m;
	ss.clear();
	ss.str("");
	m2.mantissa = m;
	m2.saveMantissa(m); 
	ff = addFloats(m1,m2);
	string mant = ff.returnMantissa();
	mant = ff.addToString(mant,1);
	ss << mant;
	m=0;
	ss >> m;
	ff.saveMantissa(m);
	ff.mantissa = m;
	return ff;
}
float16 operator * (float16 m1,float16 m2)
{

	return mulFloats(m1,m2);
}
float16 operator / (float16 m1,float16 m2)
{
	return divFloats(m1,m2);
}
istream & operator >> (istream &din,float16 &m)
{
	din >> m.input;
	m.store(m.input);
	return (din);
}
ostream & operator << (ostream &dout,float16 &m)
{
	dout<<(m).returnMantissa()<<((m.returnExponent()=="")?"":"e")<<(m).returnExponent();

	return (dout);
}
//------------------Definitions---------------------------------------------
string float16::returnMantissa()
{
	long int man=0;
	for(unsigned int i=0; i<31; i++) {
		man += atm(i)*(int)pow(2.0,(double)i);
	}
	if(atm(31) == 1) {
		man *= -1;
	}
	stringstream ss;
	string s;
	ss << man;
	ss >> s;
	return s;
}
string float16::returnExponent()
{
	string expo;
	string a;
	int n;
	for(unsigned int i=0; i<95; i++) {
		if(i == 0) {
			if(ate(i)) expo = "1";
			else expo = "0";
			continue;
		}
		a = "2";
		for(int j=1; j<i; j++) {
			a = addStrings(a,a);
		}
		if(ate(i)) {
			while(expo.size() < a.size()) expo.insert(0,"0");
			while(a.size() < expo.size()) a.insert(0,"0");
			expo = addStrings(expo,a);
		}
	}
	while(expo[0] == '0') expo.erase(expo.begin());
	if(ate(95) == 1) {
		expo.insert(0,"-");
	}
	return expo;
}
string float16::addStrings(string a,string b)
{
	int carry=0;
	string ret;
	for(int i=( a.size() -1);i>=0;i--){
		int num;
		int p=(int)(a[i]-'0');
		int q=(int)(b[i]-'0');
		num=p+q+carry;
		carry=num/10;
		num=num%10;
		num=num+(int)'0';
		ret= (char)num + ret;
	}
	if(carry!=0){
		ret=(char)(carry+ (int)'0') + ret;
	}
	return ret;
}
string float16::subStrings(string a,string b)
{
	int carry=0;
	string ret;
	for(int i=(a.size() -1);i>=0;i--){
		int num;
		int p=(int)(a[i]-'0');
		int q=(int)(b[i]-'0');
		if(carry==1){
			p--;
		}
		carry=0;
		if(p<q){
			p=p+10;
			carry=1;
		}
		num=p-q;
		num=num+(int)'0';
		ret=(char)(num) + ret;
	}
	return ret;
}
void float16::bitDisplay()
{
	/* Display the number bit-by-bit */

	cout<<"Mantissa: "<<endl;
	for(int i=0; i<4; i++) {
		for(int j=0; j<8; j++) {
			cout<<at(i,j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<"Exponent: "<<endl;
	for(int i=4; i<16; i++) {
		for(int j=0; j<8; j++) {
			cout<<at(i,j)<<" ";
		}
		cout<<endl;
	}
}
void float16::saveExponent(string expo)
{
	if(expo[0] == '-') {
		se = true;
		expo.erase(expo.begin());
	}
	string hex = toHex(expo);
	int a, b, number;
	char ch;
	for(int i=15; i > 3; i--) {
		a=0;
		b=0;
		if(hex.size() > 0) {
			ch = hex[hex.size()-1];
			if(ch >= '0' && ch <= '9') {
				a = ch - 48;
			} else {
				a = ch - 87;
			}
			if(hex.size() > 1) {
				ch = hex[hex.size()-2];
				if(ch >= '0' && ch <= '9') {
					b = ch - 48;
				} else {
					b = ch - 87;
				}
			}
			hex.erase(hex.end()-1);
			if(hex.size() > 0) {
				hex.erase(hex.end()-1);
			}
		}
		number = (16*b) + a;
		num[i] = number;
	}
	if(se) {
		num[4] = 0x80 | num[4];
	}
}
int float16::at(int row, int i)
{
	assert(i >= 0 && i < 8);

	i = 7 - i;
	int bit;
	bit = num[row] & (int)pow(2.0,(double)i);
	bit = bit >> i;
	
	return bit;
}
int float16::atm(int i)
{
	/* i==0 means 0th position of the bit - i.e. the Least Significant Bit */

	int row=0;
	int bit=0;
	while(i > 7) {
		row++;
		i -= 8;
	}
	row = 3 - row;
	bit = num[row] & (int)pow(2.0,(double)i);
	bit = bit >> i;
	return bit;
}	
int float16::ate(int i)
{
	/* same as atm(i), for the exponent */
	int row=0;
	int bit=0;
	while(i > 7) {
		row++;
		i -= 8;
	}
	row = 15 - row;
	bit = num[row] & (int)pow(2.0,(double)i);
	bit = bit >> i;
	return bit;	
}
void float16::store(string str)
{
	disseminate(str); //Disseminate the information from the string

	saveMantissa(mantissa); //Save The Mantissa at its appropriate position

	saveExponent(exponent); //Save The Exponent at its appropriate position
}
float16::float16()
{
	mantissa = 0;
	exponent = "";
	sm = false;
	se = false;
	for(int i=0; i<12; i++) {
		num[i] = 0;
	}
}
string float16::addToString(string a, int b)
{
	int sa=0;
	int sb=0;
	string ret;
	if(a[0]=='-'){
		sa=1;
	}
	if(b<0){
		sb=1;
		b=b*(-1);
	}
	string p;
	string q;
	if(sa==1){
		for(int i=1;i<a.size();i++){
			p=p+a[i];
		}
	}
	if(sa==0){
		for(int i=0;i<a.size();i++){
			p=p+a[i];
		}
	}

	int n=b;
	int num;
	while(n!=0){
		num=n%10;
		q=( (char)( num + (int)'0' )) + q;
		n=n/10;
	}
	while(p.size()!=q.size()){
		if(p.size()<q.size()){
			p='0'+p;
		} else {
			q='0'+q;
		}
	}
	if(sa==sb){
		ret=addStrings(p,q);
		string anvay;
		int flag=0;
		while(ret[flag]=='0'){
			flag++;
		}
		for(int i=flag;i<ret.size();i++){
			anvay=anvay+ret[i];
		}
		ret=anvay;
		if(sa==1){
			ret='-'+ret;
		}
	}
	if(sa!=sb){
		if(p>q){
			ret=subStrings(p,q);
		} else {
			ret=subStrings(q,p);
		}
		string anvay;
                int flag=0;
                while(ret[flag]=='0'){
                        flag++;
                }
		for(int i=flag;i<ret.size();i++){
                        anvay=anvay+ret[i];
                }
                ret=anvay;
		if(p>q){
			if(sa==1){
				ret='-'+ret;
			}
		}
		if(q>p){
			if(sb==1){
				ret='-'+ret;
			}
		}
	}
	string a1;
	if(a1==ret){
		ret='0';
	}
	return ret;
}
void float16::disseminate(string str)
{
	string s = str.substr(0, str.find('e'));
	stringstream ss;
	if(str.find('e') != string::npos) {
		exponent = str.substr(str.find('e')+1, str.length()-str.find('e'));
	}
	if(s[0] == '-') {
		if(s.find('.') != string::npos) {
			exponent = addToString(exponent,s.find('.')+1-s.size());
		}
		s.erase(s.begin());
		sm = true;
	} else {
		if(s.find('.') == string::npos) {
			;
		} else {
			exponent = addToString(exponent,s.find('.')+1-s.size());
		}
	}
	if(s.find('.') != string::npos) {
		s.erase(s.begin()+s.find('.'));
	}
	ss.clear();
	ss.str("");
	ss << s;
	ss >> mantissa;
	if(exponent[0] == '-') {
		exponent.erase(exponent.begin());
		se = true;
	}
}
string float16::toHex(string decimal)
{
	string hex = "", quot = "";
	stringstream ss;
	int i=0, num=0, q, r;
	do {
		quot = "";
		num=0;
		for(i=0; i<decimal.size(); i++) {
			num = (num*10) + (decimal[i]-48);
			q = num/16;
			r = num%16;
			quot += (char)(48+q);
			num = r;
		}
		decimal = quot;
		while(decimal[0] == '0') decimal.erase(decimal.begin());
		switch(r) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				hex = (char)(r+48) + hex;
				break;
			case 10:
				hex = 'a' + hex;
				break;
			case 11:
				hex = 'b' + hex;
				break;
			case 12:
				hex = 'c' + hex;
				break;
			case 13:
				hex = 'd' + hex;
				break;
			case 14:
				hex = 'e' + hex;
				break;
			case 15:
				hex = 'f' + hex;
				break;
		}
	}while(decimal != "");
	return hex;
}
void float16::saveMantissa(int n)
{
	if(n < 0) {
		n *= -1;
		sm = true;
	}
	int a;
	for(int i=3; i>=0; i--) {
		a = n & 0xff;
		num[i] = a;
		n -= a;
		n = n >> 8;
	}
	if(sm) {
		num[0] = 0x80 | num[0];
	}
}
//-------------------------------EOF-------------------------------------------------
