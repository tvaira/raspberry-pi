#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include "busi2c.h"

BusI2C::BusI2C(int i2c_bus/*=1*/):i2c_bus(i2c_bus),i2c_fd(0),current_slave(0)
{
}

int BusI2C::open()
{
	char buff[32];

	if (!i2c_fd) 
   {
		sprintf(buff, "/dev/i2c-%d", i2c_bus);

#ifdef I2C_DEBUG
		printf("\t\t\topen() : %s\n", buff);
#endif

		i2c_fd = ::open(buff, O_RDWR);

		if (i2c_fd < 0) 
      {
			perror("open(i2c_bus)");
			i2c_fd = 0;
			return -1;
		}
	}

	return 0;
}

void BusI2C::close()
{
	if (i2c_fd) 
   {
		::close(i2c_fd);
		i2c_fd = 0;
		current_slave = 0;
      #ifdef I2C_DEBUG
		printf("\t\t\tclose()\n");
#endif
	}
}

int BusI2C::select_slave(unsigned char slave_addr)
{
	if (current_slave == slave_addr)
		return 0;

	if (this->open())
		return -1;

#ifdef I2C_DEBUG
	printf("\t\tselect_slave(%02X)\n", slave_addr);
#endif

	if (::ioctl(i2c_fd, I2C_SLAVE, slave_addr) < 0) 
   {
		perror("ioctl(I2C_SLAVE)");
		return -1;
	}

	current_slave = slave_addr;

	return 0;
}

void BusI2C::set_i2c_bus(int bus)
{
	if (i2c_fd)
		this->close();

	i2c_bus = bus;
}

int BusI2C::write(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char const *data)
{
	int result, i;

	if (length > MAX_WRITE_LEN) 
   {
		printf("Max write length exceeded in write()\n");
		return -1;
	}

#ifdef I2C_DEBUG
	printf("\twrite(%02X, %02X, %u, [", slave_addr, reg_addr, length);

	if (length == 0) 
   {
		printf(" NULL ] )\n");
	}
	else 
   {
		for (i = 0; i < length; i++)
			printf(" %02X", data[i]); 

		printf(" ] )\n");
	}
#endif

	if (select_slave(slave_addr))
		return -1;
   
	if (length == 0) 
   {
		result = ::write(i2c_fd, &reg_addr, 1);
      #ifdef I2C_DEBUG
      printf("\tresult : %d\n", result);
      #endif
		if (result < 0) 
      {
			perror("write:1");
			return result;
		}
		else if (result != 1) 
      {
			printf("Write fail:1 Tried 1 Wrote 0\n");
			return -1;
		}
	}
	else 
   {
		txBuff[0] = reg_addr;

		for (i = 0; i < length; i++)
			txBuff[i+1] = data[i];

		result = ::write(i2c_fd, txBuff, length + 1);

		if (result < 0) 
      {
			perror("write:2");
			return result;
		}
		else if (result < (int)length) 
      {
			printf("Write fail:2 Tried %u Wrote %d\n", length, result); 
			return -1;
		}
	}

	return 0;
}

int BusI2C::read(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char *data)
{
	int tries, result, total;

#ifdef I2C_DEBUG
	int i;

	printf("\tread(%02X, %02X, %u, ...)\n", slave_addr, reg_addr, length);
#endif

	if (this->write(slave_addr, reg_addr, 0, NULL))
		return -1;

	total = 0;
	tries = 0;

	while (total < length && tries < 5) 
   {
		result = ::read(i2c_fd, data + total, length - total);

		if (result < 0) 
      {
			perror("read");
			break;
		}

		total += result;

		if (total == length)
			break;

		tries++;		
		delay_ms(10);
	}

	if (total < length)
		return -1;

#ifdef I2C_DEBUG
	printf("\tLeaving read(), read %d bytes: ", total);

	for (i = 0; i < total; i++)
		printf("%02X ", data[i]); 

	printf("\n");
#endif

	return 0;
}

int BusI2C::read(unsigned char slave_addr, unsigned char length, unsigned char *data)
{
	int tries, result, total;

#ifdef I2C_DEBUG
	int i;

	printf("\tread(%02X, %u, ...)\n", slave_addr, length);
#endif	

   if (select_slave(slave_addr))
		return -1;

	total = 0;
	tries = 0;

	while (total < length && tries < 5) 
   {
		result = ::read(i2c_fd, data + total, length - total);

		if (result < 0) 
      {
			perror("read");
			break;
		}

		total += result;

		if (total == length)
			break;

		tries++;		
		delay_ms(10);
	}

	if (total < length)
		return -1;

#ifdef I2C_DEBUG
	printf("\tLeaving read(), read %d bytes: ", total);

	for (i = 0; i < total; i++)
		printf("%02X ", data[i]); 

	printf("\n");
#endif

	return 0;
}

int BusI2C::delay_ms(unsigned long num_ms)
{
	struct timespec ts;

	ts.tv_sec = num_ms / 1000;
	ts.tv_nsec = (num_ms % 1000) * 1000000;

	return nanosleep(&ts, NULL);
}

int BusI2C::get_ms(unsigned long *count)
{
	struct timeval t;

	if (!count)
		return -1;

	if (gettimeofday(&t, NULL) < 0) 
   {
		perror("gettimeofday");
		return -1;
	}

	*count = (t.tv_sec * 1000) + (t.tv_usec / 1000);	

	return 0;
}
