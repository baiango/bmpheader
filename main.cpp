#include "bmpheader.hpp"


/* Code coverage
clang++ -fprofile-instr-generate -fcoverage-mapping main.cpp
a
llvm-profdata merge -sparse default.profraw -o default.profdata

// Detailed
llvm-cov show a.exe -instr-profile=default.profdata
// Summary
llvm-cov report a.exe -instr-profile=default.profdata
// HTML
mkdir coverage_report && llvm-cov show -format=html -o coverage_report -instr-profile=default.profdata a.exe
*/

/*
// It trigger on std::string or std::to_string(), so don't use the ASan
//clang++ -g -O1 -D LIBFUZZER -fsanitize=fuzzer,address -fno-omit-frame-pointer main.cpp
//set ASAN_OPTIONS=detect_container_overflow=0
clang++ -g -O1 -D LIBFUZZER -fsanitize=fuzzer -fno-omit-frame-pointer main.cpp
a
*/
uint64_t iteration_counter = 0;
extern "C" int LLVMFuzzerTestOneInput(const uint8_t Data[], size_t Size) {
	if (Size < 4 * 4) {
		return 0;
	}
	using namespace BmpHpp;

	Img24 bmp(512, 512);

	// Check that fuzzer is working
	for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++) {
		for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++) {
			Result<Img24 *, bool> pixel = bmp.xy(x, y);
			if (pixel.is_ok())
				pixel.get_value()
				->r(x)
				->g(y * 10)
				->b(x * 30);
		}
	}

	int32_t x, y;
	uint8_t r, g, b;

	for (uint32_t i = 0; i < 1000; i++) {
		size_t data_index = i * 1;
		if (data_index + 16 > Size)
			continue;

		memcpy(&x, &Data[0 + data_index], 4);
		memcpy(&y, &Data[4 + data_index], 4);

		memcpy(&r, &Data[8 + data_index], 1);
		memcpy(&g, &Data[9 + data_index], 1);
		memcpy(&b, &Data[10 + data_index], 1);

		x = (uint32_t)x % bmp.get_width();
		y = (uint32_t)y % bmp.get_height();
		Result<Col888 *, bool> pixel = bmp.px_ptr(x, y);
		if (pixel.is_ok())
			*(pixel.get_value()) = {r, g, b};
	}
	bmp.save(std::to_string(iteration_counter) + ".bmp");
	iteration_counter++;
	return 0;
}

void read_and_save_unit_test() {
	using namespace BmpHpp;
	{
		Img24 bmp(512, 512);

		bmp.save_sample();

		bmp.clear_pixel();
		if (not bmp.read("sample.bmp"))
			std::cerr << "Reading sample.bmp failed!" << std::endl;
		bmp.save("read.bmp");
	}
	{
		Img24 bmp;

		bmp.read("sample.bmp");
		bmp.save("aaa.bmp");
	}
}

void md_example() {
	using namespace BmpHpp;

	for (int32_t count = 20; count <= 45; count += 1) {
		Img24 bmp(count, count / 2);

		for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++)
			for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++)
				bmp.xy(x, y).get_value()
				->r(x)
				->g(y * 10)
				->b(x * 30);

		bmp.save(std::to_string(count) + ".bmp");
	}
}



void heatmap_unit_test2() {
	using namespace BmpHpp;
	using std::array;

	array<array<uint64_t, 16>, 16> bucket;
	Heatmap heatmap;
	uint64_t num = 1000;

	// Hashmap here
	// Example: hash() % bucket_size
}

void init() {

}

#ifndef LIBFUZZER
int main() {
	//read_and_save_unit_test();
	//md_example();
	//heatmap_unit_test2();
}
#endif


// See blake3_reader.cpp
std::string rng_num = "B-j(Fv#?HKa7RPRCkrOSK3}#?(_=s0SZ|PMVF*igDc~t8%WB-Zc~T1?twm~0g?wQrYA(W}fOfyX#lu>ZbVntSK>zcUd3Z#hahO*cw<Q}GY^t0n%X1+rBoK9kjdctF+CnCB6CBULY>4Ssbr^u`D$-g~4%>7b*WEl7c;!72(4*apQFO=m?ME9|8`K9j67+C<lz?ZERv4L2=C1De1Yl*RRb}0Ipn$eJxrA%j`qN0!*l5~eEn0|nZUKS!FIAUZ7h+BeRB>2h!3F@E%%`~*W-VfDCOd5CCGJzZGXH}7{K_Wk(UzgwBT{y8z1FgRANFOjyO(Tzh8V3?mzp#Ip>Q_L#WHQ<lUOD$S|rfj7--sP^M7oYmz(0*OXQu?-!CrLz&@92Z5eEFKP$7u6IsKNqV4F+Jr_XE(j#YW@*Q%YDm9}c&`~*rN|-^@Q|VTqkC-6PEQ*xGeemr$XdXGaiqhA&7qY}^4_Pn(@m}slkh~4#wh!$P?-8r$OVd_u>I@rN=+|E=#Get~ubdclYn&QP0*}+LUzmGMfw{+T0?%p}Nsm+IfEl7M&sGmROPnPhJAJWyxSlXL&Bq0_{Dg0KXX5)6fpG0PK<|!1iyXrKfH0KIu^^?$XH@e0llPFl(jE`CaZaO&F3ncnzy5>4BgTyF7Ud5g@77Y#c$1G+GGcKdPJrmFrBuEv&wFrFPQ+q=T?@VK5kfpbhT$%6ZuZZR1Zi&20PRF#?*p^1haCE8i5SNFsM)GLR~htS9^YWNxmWTb`#LBvOtvwtv4<JHZ(KKe_}oSzcSvWBh3(40toT&UyXPnAkSlG53LhD;1kZF~DV4eDxkM6l>*#H;ejqWAb<v@GNLAm4U4>{_5vq{N<}@%V)_Ljg!}#OS%VLB${No>ID(!`#LYgPTKOK}rpL|<4Aav}AqvP=d+6H}k3(ZIXhmnbYobb(L8V5ca5M^5gsN#H&=@XF`hkh>jC&K9AMbqB!GNi$G(VDp{E~F1T_@$39iy5tKpYJ=K|Eg^lo4S{Ub~#bvVkE9n2k7$dNb-P`JiVNo?~|1yq1*rYB#bi&c&e!+GQR?K7`epVYd7t%KGVjP75<(vZ0N?aU*gM+nOKXjfhF7%M^Ics3Kdu20RcjTk(W_NFP|DkoyM?};gysWAp8YXCTGPX7%BHb|I)MUS)%%9@o}}$;JMu;+8@B)ojh&|9Y3wN+ddX8%tWW6W++j5AN4OD*a%6t9R2F_BoI<Il#;=lRk+M;Vt~0=+bNDBnQXLhHmKMfop@z*&$Q<ITSy7ufoPZ3og3$`&Y7wmr!iHfvn=VTTmuO&maEZ0fT~xq$L8tk?LIdvb?7}W8WAfUK2MnU33|bBRH_-a!=FF;6Z8oz)1E`4^Kf|tT=Z?SO{s-B@%m=!f-`8|T}4Dx_L6-beL;^jPUp)>?B>QPWpqJ@=Q-12Z?J+!p{Y3NT6>HnBUtd=JF<pu^kXq!Nb@tzci&@km_XAQ>XBqfIN4GFLMsEt*irL6p*vE+=Rm&F9ymo8OIfT0aZ@3Gd?x})Z`ymy?61G!vP^g;yhivuH~NjSTwIRIe7Azf;LRNbzD=jrlQnAe<+bGszJ<|-DUSD-7g2se6|#xOVP{YZxHY+keIBuo7HcEPMl#dT8zq!5zrffN+)dy-54B;^MskpeGEZ2(mMRSJ5^uvJ_?rzRts<obEkY3_H9%z@(EV)*H0GpgSllAGnSW35Gsx?XnJrgg`o)2ONNd^5riaDt76yI%u|$i<`^fu3S&;UA3OQ)LsS}yjwL~=GIS6Otl~J5>pmn@VYA@y|vSjO0fy66FP!8t8M^K()cLb-Q<cGz!$uAQ~Ft1JHq%wq(pKbmk*Ki>a#g14c__t{z%$nkCT87I6^6sahnA?N;d}#jD#~%iz>_DiW8WB~DV8^>k7BZ=^x~(9yCB1Is>fPQV@ZZYb2W+5x-M5BwHxH-CPbI4U4|1VgWS5~+mOJ`{<3l6A+tCsZ4huSX%$>5K0fDlKcPf`$cw5K<gib4IbOwNi^5*=xQ4*W4AD0Q6=&;1ZbBc2SKa#Slsv~e`CSmiLSmqVIr30zJtYY0|bvm&}RRIZ=jiRu&hxXHSjOmQ)EVB1t`>c;5Ek?AhRxfRRRX)UWcVCelGpTBrch*Jn9Px5g42Yw%K-x*igqz!7HMn9a-PSSkSy$=DRM$&MQ^?$=#f%xFWl^hZVp>~^cg=VDP1EaeEcGL_Z(G*<HB@Hk^>0Yl)*`3Om@B2?#5@A$?VS$7)XObYbgYjEaTKu34yO6RStnK<i_Cn>C*soP9SGbV9?qg+V0+smpRJ$RuUz5kNz!!;h&P=cIDfawY6Iaamy|M-Nh3Y2ToAR`N%~1c-VRVjP~RC2ciIB_UR>E-iB%lmT#a`rQZfJ=NXOCPWsYUHOzGkKR%UE1B315c%9L|wG<mz0{#;USUY#Zc$(ry2j#NxyQb|jJ&R5&%$hmb!xDP}}ONF$+>H4xC#e5|zzH7G>fv>#7OpuglkQa*-O@1Ru+gjQEq6?yr(Fc=(Pa68$Hjz>GUh-KqE-nZd{lleAE_`|`o%sU2>f_#CrzB!22)#`GkYcf;*2a>I-zVi=09%p$hWkN%Q6K<l!H_!*Re>)d-1zu?9a(lnck?-V-d}Lok~XH7%n_kXe`5fysuDmcgCC_;M!5g+mSxaVf(_&aZ~QA%|Gi{;AHMs6h?8WoqqJ<Nwl-*rN?=k`Q7+ir6hebf>~a^@oM&22`o9T2uD-&j1)63Um=JjZD<C^~BUvA~tzuyZH(Bu<E&5vADWqvSsMACW{*p8)M5jHJGM-6f{<d(bF>a->iP5`%@){5Jd?~Vf+o)jwEtWW{eKS{5H2eJenf3X^%lL68r6MT=h;-Mczpx(sIYpjjw_7cT<OfSKM(GfeH>LqFD}=;;fjFJfHkTxVY((Qu((z|p17G~QDnIh188gX<SQl>J@IMj6&nv}A!_;<8zJXoZeBpqwsXK8}?nVhPy?Dran)(I8NK&G-hxa8<izj)yL+LQ=O`NaEZ|O9r-Icl)Kp9bgd4<2=aA=r&h-+MS7c9kT&_?)22B@cO7dfuYmpAfjoFL}uDh$)q$Bpp%l-E(}!^gK>ZZy1^5pA(&JQ_RXQ@!F}Qw|n}r7O@2&GGLfLj1{e<mP<0tcq--9g*Fi>O47Qs?m|;O)0nlci^M5zgV^xe{7BtefHXX!4O~OQZ(h3kOF<`JB?y{Z9=bp0XP6i4iTp(2gP52&K9g`bM?EmMaW3MZ7(Zx^~De7w1*Jf<f>F3*h1ofdmR?b|2zL|)yu=fVKYcp&`wPRCc|XUU**LP^5Vj=^v#u)2umWIc;aJc4pE+1Lbq1|)q2ML6v>QctCmz-9y%RyW3`*UHp!)&x-^3Qi6Ix6;-am2P50UPV#!;^37`A#gd7*?3p#(|A2<eT0MOM$hv;UG<b;ynn^i)P(Nb$YV<o`xHp?e+38}UYi0(?jcbKn>N@bMrn-1Xrf3McM&09(>h;i2P+!NxMaD3c3>9;rB{vV?csB@VBc@F8@2hY4IY3%-VUET0&eAPSlpqjhEU|V&pD{L~2dtmp}zA;7&B}P)4=j@6PbX-)Is?M`JpA|U-3LSG14hJB~nO!4BpL3NxEg_YA#cipcrPr8@mmrW&HiD9a3@J5p3yf7{MzV+X5(vamrT{D3Or|VoginbggnhAM4T+eR@8X(G(Ot<(d>vNX9#%m`#fZ-uC@8}&`rQTVfW-uGo&}7Nis9K=r_H#a%PSu!BvQSmFWRg?1HobSq55*VFnjz`G$o7hx4=FfJm1Ro%<26<K*cN^L4`5E2f<e7TXcB%AS5(xqUCrmqL0;z&<i%?BRn<Uegt|m7KMD0lO5mx+_MECxnwgRG4d-8kuXGSU1?Vp<5QzU<)WE@C-~<^MpUK+$+KAbhSzp<3$kA5E@vfhaEnFCEIl0Hq9S}p>u^a_?<<v*u%P@u8Dqof@8D>*$V0`&{xSm6gD$c?k|eEB)NQs1WUMb5S56(Du`q2#U^5dTKYds}xVG9)HY0^sg;%HaFsjnFw+6IhU070ZC%-F3dyQS?crUeEK-m4JF*d~xQ$3CX^ZZV-Q3>yF%Mu%Rrb^+qi!?IrUr3n9H|AKi#nI?}aA4Cv*5V_U20Mei!yZyl#Q!%P4;qjWio1a)*JyGe8dm3eX_!#K!gjHYxwwaWgZ_F=eB@#ng5}o`H)R9;B*w6df9w2Gc?8Q0(cHOGa!x}q*RyF_YYEj7DvtF`P%xp3w06UD4oLCEhn{Sg>ZwR2vs8qSd(|_x4rq_T0{90Hj5U1zCBf%ZyDvLzltPOu%!)qCq63IriRkeLz7swXr5&^xWBY2RMJERo@qk@TFC~}2G%RI-?|N3$N>05oG;4as>T3htLg;&~Co|dvOWkcwS;q5GsoOEsP(AVxp;3Ck*KQl<9bJ`?A^Qual+YEe8uX>1lSx(5rr#IM=1MLNBupCRgMRB&a6BeR%GlV2^0-u;h{!F&_s9@N0J>nj-u+OQAR>YpNOyQykePm;8hpvx%->-&(nn^uIqOpdzIc2TobXc>)cLXNv6{y96KTKee}5g;6XvyAg{}L*CK{Jh0s}?GlZQ`Ch$fOCmS&NZtS*BPb^!)1FCHc}Tq4s7EW}}hKn+)qbc&?7+C`<+;$|R+tbX%9(CTK9u-0V#icJI2l>as`lKgN&t`lJRMlKsJ2MOG$L|EEP!mZj$+`q_pT}fgaU_dsOja*A2ENUKNhD!VXp5ls$r=R6X??-um<;Y{Ch~%#*ATV0WEo4A*8m4ur_+D7f4GJ5+wj;t8exTq!t)KRyns!-Asg?KI7q+0#(1VyyX^2-GAK2kIwlGrJn(=M#bIBMKcQ@k)uO1zIN9zPQSTcHN=Dhcpw99Y7<2>#okbPIYnNu|HH)PmYm`;*1E)`P5at&F_qHAp#@Vkh@8XWT7@B15g^#h_k9d-Xm!1nVb>(<!Z-6rIkbUYKxdF(h?Tj{mL42LG+rL@mYwSTFhR*Fy}jQmsVMzU_O<I9EqqE6$S+#{l1)KYno@j{32EqbJa9x2t~)Nb&4%IT?!A7z_1`tb4=LS>5aw$8{Tg+V>9M9-;&P<T@!%zQtl{+FVn&dAL%C^cO^7BIciMH%f$2BOF!{N&QKfb!4Dap5NG;)Hi1eXeTJDa|Fnh&W^%$uXu{_$t_pbFTDAZk>5zCKxNTqQzlI&V{1<{)76qA_k`_!=EPjo72>amdn>|MM+XggPHEX";
