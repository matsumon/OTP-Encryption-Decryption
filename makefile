run:
	p4gradingscript 9999 99999 > mytestresults 2>&1	
all:
	make keygen encrypt decrypt
keygen:
	gcc keygen.c -o keygen
encrypt: 
	gcc otp_enc_d.c -o otp_enc_d
	gcc otp_enc.c -o otp_enc
decrypt: 
	gcc otp_dec.c -o otp_dec
	gcc otp_dec_d.c -o otp_dec_d
clean:
	rm -rf otp_enc otp_enc_d otp_dec otp_dec_d keygen
kill:
	pkill -9 otp_dec_d
	pkill -9 otp_enc_d
