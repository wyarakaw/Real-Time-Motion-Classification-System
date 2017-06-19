CC=gcc
CFLAGS=-Wall
LDFLAGS= -lmraa -lm -lfann
SOURCES= consumer.c producer.c classifiers.c constants.c LSM9DS0.c features_2.c main.c strides_2.c training_file.c turnig_file.c prompt_collect.c training_data_new.c test_neural_network.c train_neural_network.c last_functions.c time_features.c time_features.c

EXECUTABBLES= producer.c consumer.c main.c

all: producer consumer main

producer: producer.c LSM9DS0.c constants.c constants.h 
	$(CC) $(CFLAGS) -o producer -w producer.c LSM9DS0.c constants.c classifiers.c $(LDFLAGS)

consumer: consumer.c constants.c constants.h classifiers.c features_2.c strides_2.c test_neural_network.c last_functions.c time_features.c
	$(CC) $(CFLAGS) -o consumer -w consumer.c constants.c classifiers.c features_2.c strides_2.c last_functions.c test_neural_network.c time_features.c $(LDFLAGS)

main: main.c LSM9DS0.c constants.c classifiers.c features_2.c strides_2.c training_file.c turning_file.c prompt_collect.c training_data_new.c train_neural_network.c last_functions.c time_features.c
	$(CC) $(CFLAGS) -o main -w main.c LSM9DS0.c constants.c classifiers.c features_2.c strides_2.c training_file.c turning_file.c prompt_collect.c training_data_new.c train_neural_network.c last_functions.c time_features.c $(LDFLAGS)


clean:
	rm -f producer
	rm -f consumer
	rm -f main
