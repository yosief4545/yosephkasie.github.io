from pymongo import MongoClient
from bson.objectid import ObjectId
from pymongo.errors import PyMongoError


class AnimalShelter(object):
    """Enhanced CRUD operations for Animal collection in MongoDB"""

    def __init__(self, username, password):
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 33827
        DB = 'AAC'
        COL = 'animals'

        try:
            self.client = MongoClient(
                f'mongodb://{username}:{password}@{HOST}:{PORT}',
                serverSelectionTimeoutMS=5000
            )
            self.database = self.client[DB]
            self.collection = self.database[COL]

            # Test the connection
            self.client.server_info()

        except PyMongoError as e:
            raise ConnectionError(f"Database connection failed: {e}")

    def create(self, data):
        """Insert one document into the collection."""
        if not data or not isinstance(data, dict):
            raise ValueError("create() requires a non-empty dictionary")

        try:
            result = self.collection.insert_one(data)
            return result.acknowledged
        except PyMongoError as e:
            print(f"Create error: {e}")
            return False

    def read(self, query=None):
        """Read documents matching a query. If no query is given, return all documents."""
        if query is None:
            query = {}

        if not isinstance(query, dict):
            raise ValueError("read() requires a dictionary query")

        try:
            results = self.collection.find(query)
            return list(results)
        except PyMongoError as e:
            print(f"Read error: {e}")
            return []

    def read_by_id(self, document_id):
        """Read one document by MongoDB ObjectId."""
        try:
            result = self.collection.find_one({"_id": ObjectId(document_id)})
            return result
        except Exception as e:
            print(f"Read by ID error: {e}")
            return None

    def update(self, query, new_values):
        """Update documents matching the query."""
        if not isinstance(query, dict) or not query:
            raise ValueError("update() requires a non-empty dictionary query")

        if not isinstance(new_values, dict) or not new_values:
            raise ValueError("update() requires a non-empty dictionary of new values")

        try:
            result = self.collection.update_many(query, {"$set": new_values})
            return result.modified_count
        except PyMongoError as e:
            print(f"Update error: {e}")
            return 0

    def delete(self, query):
        """Delete documents matching the query."""
        if not isinstance(query, dict) or not query:
            raise ValueError("delete() requires a non-empty dictionary query")

        try:
            result = self.collection.delete_many(query)
            return result.deleted_count
        except PyMongoError as e:
            print(f"Delete error: {e}")
            return 0