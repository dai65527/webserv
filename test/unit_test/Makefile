DIRS :=	CommonConfigStore\
		ConfigParser\
		MainOnlyConfigStore\
		ParseRequest\
		ServerLocationConfigStore\
		ServerOnlyConfigStore\
		Session\
		UrlEncoder\
		webserv_utils

.PHONY: test
test:
		set -e; \
		for dir in $(DIRS); do \
			echo $$dir; \
			make -C $$dir; \
		done

.PHONY: fclean
fclean:
		for dir in $(DIRS); do \
			make fclean -C $$dir; \
		done

.PHONY:	re
re:		fclean test
