# Kupinggajah
## Indonesian Open Source Secure Chat Platform: Chat Server

### Dependencies:
C Libraries/ Binaries Distributions
```bash
kore lnats lpthread ldl lhiredis ljson-c
libtool autoconf automake
```
Tambahan dependencies jika menggunakan fitur nats-streaming-server yg tidak ada di gnatsd:
```bash
libprotobuf-dev  libtool autoconf automake
```

### Development Notes:
Sudah dites di Linux amd64 Ubuntu LTS 18.04.

Getting Started:

Install dependensi, Redis, dan jalankan NATS Server
```bash
sudo apt install autoconf automake libtool redis # Redis 4.0 up
wget https://github.com/nats-io/nats-streaming-server/releases/download/v0.14.3/nats-streaming-server-v0.14.3-linux-amd64.zip
unzip nats-streaming-server-v0.14.3-linux-amd64.zip
# pada versi nats streaming server ini, masih compatible dengan protokol NATS 1.0
# dan kompatibel dengan fitur gnatsd
cd nats-streaming-server-v0.14.3-linux-amd64
chmod +x nats-streaming-server
./nats-streaming-server -D &

sudo systemctl start redis
```

Salin semua dependensi, compile dan install di sistem anda.
```bash
git clone https://github.com/jorisvink/kore.git     # Kore 3.3.1
git clone https://github.com/nats-io/nats.c.git     # NATS C June 2, 2019
git clone https://github.com/redis/hiredis.git      # Hiredis 1.0
git clone https://github.com/json-c/json-c.git      # Json-C June 10, 2019

cd kore
export NOTLS=1
make
sudo make install

cd ../nats.c
mkdir build
cd build
cmake .. -DNATS_BUILD_WITH_TLS=OFF -DNATS_BUILD_STREAMING=OFF #(hilangkan -DNATS_BUILD_STREAMING=OFF jika anda menginginkan untuk dukungan fitur nats-streaming-server yang tidak ada di gnatsd)
make
sudo make install

cd ../../hiredis
mkdir build
cd build
cmake ..
make
sudo make install

cd ../../json-c
sh autogen.sh
./configure
make
sudo make install

# Finishing
sudo ldconfig
```
Untuk menjalankan Server Kupinggajah, pastikan firewall anda allow port 30000
```bash
git clone https://github.com/aviezab/kupinggajah.git
cd kupinggajah/kupinggajah-server
kodev clean && kodev run
```

Untuk compile Worker dan menjalankan
```bash
gcc subq1.c natshighsend.c redishigh.c  -L/usr/local/lib -L. -I/usr/local/include -I/usr/local/include/hiredis  -Wl,--as-needed -ldl -lnats -lpthread -lprotobuf -ljson-c -lhiredis -o worker1
./worker1
```

## Architecture
![alt text](https://grit.id/github/kupinggajah-arch.png)
## Status Development:
Server kupinggajah sudah bisa menerima pesan, meroutingnya, dan client sudah bisa mengambil pesan
dari pengirim.
![alt text](https://grit.id/github/first_milestone_20190527.png)

Worker sudah menjalankan fungsinya sebagai router.
![alt text](https://grit.id/github/first_milestone_sub_20190527.png)

## Milestones Pengembangan (dalam progress)
- Bisa menerima pesan √
- Bisa mengoper pesan ke MQ1 √
- Worker bisa mengambil pesan dari MQ1 lempar ke MQ2 √
- User bisa mengambil pesan dari user lain yang mengirim √
- Database user untuk registrasi, login dan daftar kontak
- Fungsi untuk register, login dan list kontak
- Security: validasi dan enkripsi baik transport HTTPS dan cipher
- Front-end berupa web dan mobile app
- Tes benchmark untuk konkurensi per detik dengan ukuran pesan tertentu
- Untuk upload gambar dan file, akan dihandle oleh front-end karena menggunakan back-end terpisah

## Mengapa menggunakan C?
Karena kami bisa. C juga memiliki performa tinggi dengan machine code yang sedikit.

## Anda ingin berkontribusi?
Anda bisa cari dan chat kami di grup Telegram: https://t.me/idcplc , https://t.me/CCpp_Indonesia

## License
Dibuat di bawah lisensi MIT.

Inisiasi dilakukan oleh teman-teman dari grit.id . Dimulai dari hal kecil di Indonesia, untuk dunia.

© Copyleft 2019, inisiator: aviezab.