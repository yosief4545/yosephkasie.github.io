from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self, username, password):
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 33827
        DB = 'AAC'
        COL = 'animals'

        self.client = MongoClient(f'mongodb://{username}:{password}@{HOST}:{PORT}')
        self.database = self.client[DB]
        self.collection = self.database[COL]

    def create(self, data):
        """Insert a new document into the collection."""
        if data:
            try:
                result = self.collection.insert_one(data)
                return result.acknowledged
            except Exception as e:
                print(f"Insert error: {e}")
                return False
        else:
            raise ValueError("Empty data provided")

    def read(self, query):
        """Find documents matching a query and return them in a list."""
        try:
            results = self.collection.find(query)
            return list(results)
        except Exception as e:
            print(f"Read error: {e}")
            return []

    def update(self, query, new_values):
        """Update documents matching the query with new values."""
        try:
            result = self.collection.update_many(query, {"$set": new_values})
            return result.modified_count
        except Exception as e:
            print(f"Update error: {e}")
            return 0

    def delete(self, query):
        """Delete documents matching the query."""
        try:
            result = self.collection.delete_many(query)
            return result.deleted_count
        except Exception as e:
            print(f"Delete error: {e}")
            return 0



