import redis

class RedisClient:
    @staticmethod
    def get_client(host='localhost', port=6379, db=0):
        """
        Creates and returns a Redis client instance.
        :param host: Redis server host (default is 'localhost')
        :param port: Redis server port (default is 6379)
        :param db: Redis database number (default is 0)
        :return: Redis client instance
        """
        return redis.StrictRedis(host=host, port=port, db=db, decode_responses=True)

    @staticmethod
    def set_value(key, value, host='localhost', port=6379, db=0):
        """
        Sets a value in Redis for the given key.
        :param key: The key for the Redis entry
        :param value: The value to set for the key
        :param host: Redis server host (default is 'localhost')
        :param port: Redis server port (default is 6379)
        :param db: Redis database number (default is 0)
        """
        try:
            redis_client = RedisClient.get_client(host, port, db)
            redis_client.set(key, value)
            return 1
            # print(f"Successfully set {key} = {value}")
        except Exception as e:
            return None
            # print(f"Error setting value: {e}")

    @staticmethod
    def get_value(key, host='localhost', port=6379, db=0):
        """
        Gets a value from Redis for the given key.
        :param key: The key whose value to fetch
        :param host: Redis server host (default is 'localhost')
        :param port: Redis server port (default is 6379)
        :param db: Redis database number (default is 0)
        :return: The value associated with the key or None if key does not exist
        """
        try:
            redis_client = RedisClient.get_client(host, port, db)
            value = redis_client.get(key)
            if value is not None:
                return value
            else:
                print(f"Key '{key}' not found in Redis.")
                return None
        except Exception as e:
            print(f"Error getting value: {e}")
            return None



