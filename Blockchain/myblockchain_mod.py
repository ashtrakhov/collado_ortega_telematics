from hashlib import sha256
from flask import Flask, jsonify, request
import requests, json, time, copy

class Block:
    def __init__(self, index, transactions, timestamp, previous_hash, nonce = 0):
        self.index = index
        self.transactions = transactions
        self.timestamp = timestamp
        self.previous_hash = previous_hash
        self.nonce = nonce

    def compute_hash(self):
        block_string = json.dumps(self.__dict__, sort_keys = True)
        return sha256(block_string.encode()).hexdigest()

class Blockchain:
    difficulty = 4
    max_unconfirmed_txs = 4

    def __init__(self):
        self.unconfirmed_transactions = []
        self.chain = []
        self.create_genesis_block()

    @property
    def last_block(self):
        return self.chain[-1]

###################################################################################################################################################
################################################################# New/Tweaked Methods #############################################################
###################################################################################################################################################

    def create_genesis_block(self):
        genesis_block = Block(0, [], 0, "0")
        # We could try to hardcode the value (nonce = 25163) here as the Genesis Block is always the same!
        genesis_block.hash = self.proof_of_work(genesis_block)
        # genesis_block.hash = genesis_block.compute_hash()
        self.chain.append(genesis_block)

    @property
    def unconfirmed_tx(self):
        return len(self.unconfirmed_transactions)

    @property
    def chain_cpy(self):
        # We also need to copy references as we'll be pop()ing out the last transaction when validating the chain!
        return [copy.deepcopy(blk) for blk in self.chain]

    @classmethod
    def check_chain_validity(cls, chain):
        result = True
        previous_hash = "0"

        # We've only got the Genesis node. This way we don't impose the difficulty to the Genesis itself!
        # if len(chain) == 1:
        #     return True

        for block in chain:
            block_hash = block.hash

            delattr(block, "hash")

            # Remove the reward for mining the block!
            try:
                block.transactions.pop()
            except IndexError:
                # Ignore the Genesis block
                pass

            if (not cls.is_valid_proof(block, block_hash) or previous_hash != block.previous_hash) and block.timestamp != 0:
                result = False
                break

            block.hash, previous_hash = block_hash, block_hash

        return result

    @staticmethod
    def change_difficulty(diff):
        Blockchain.difficulty = diff

    @staticmethod
    def change_max_pending_txs(pend):
        Blockchain.max_unconfirmed_txs = pend

###################################################################################################################################################
###################################################################################################################################################
###################################################################################################################################################

    def add_block(self, block, proof):
        previous_hash = self.last_block.hash

        if previous_hash != block.previous_hash or not Blockchain.is_valid_proof(block, proof):
            return False

        reward_trx = {
            'Sender': "Blockchain Master",
            'Recipient': "Node_Identifier",
            'Amount': 1,
            'Timestamp': time.time(),
        }

        blockchain.add_new_transaction(reward_trx)

        block.hash = proof
        self.chain.append(block)
        return True

    @staticmethod
    def proof_of_work(block):
        computed_hash = block.compute_hash()
        while not computed_hash.startswith('0' * Blockchain.difficulty):
            block.nonce += 1
            computed_hash = block.compute_hash()

        return computed_hash

    def add_new_transaction(self, transaction):
        self.unconfirmed_transactions.append(transaction)

    @staticmethod
    def is_valid_proof(block, block_hash):
        return block_hash.startswith('0' * Blockchain.difficulty) and block_hash == block.compute_hash()

    def mine(self):
        if not self.unconfirmed_transactions:
            return False

        last_block = self.last_block

        new_block = Block(index = last_block.index + 1,
                          transactions = self.unconfirmed_transactions,
                          timestamp = time.time(),
                          previous_hash = last_block.hash)

        proof = self.proof_of_work(new_block)
        
        self.add_block(new_block, proof)

        self.unconfirmed_transactions = []

        return True

app = Flask(__name__)

blockchain = Blockchain()

peers = set()

###################################################################################################################################################
############################################################### New/Tweaked Endpoints #############################################################
###################################################################################################################################################

@app.route('/change_difficulty', methods = ['POST'])
def update_difficulty():
    tx_data = request.get_json()
    if not tx_data.get("n_diff"):
        return "Invalid data...", 400

    n_diff = tx_data['n_diff']

    if n_diff <= 0:
        return "Bad value...", 400

    blockchain.change_difficulty(n_diff)

    return "New difficulty -> {}".format(n_diff), 201

@app.route('/change_max_pending_txs', methods = ['POST'])
def update_max_pending_txs():
    tx_data = request.get_json()
    if not tx_data.get("n_pend"):
        return "Invalid data...", 400

    n_pend = tx_data['n_pend']

    if n_pend <= 0:
        return "Bad value...", 400

    blockchain.change_max_pending_txs(n_pend)

    return "New maximum pending transactions -> {}".format(n_pend), 201

@app.route('/validate_chain', methods = ['GET'])
def validate_chain():
    # If passed by reference we'll be overwriting the original chain!
    if blockchain.check_chain_validity(blockchain.chain_cpy):
        return "Valid chain!", 200
    return "The chain has been tampered with!", 200

@app.route('/tamper_chain', methods = ['POST'])
def tamper_chain():
    tx_data = request.get_json()

    if  tx_data.get("blk_n") == None or tx_data.get("tx_n") == None or tx_data.get("new_recipient") == None:
        return "Invalid data...", 400

    blk_n = tx_data['blk_n']
    tx_n = tx_data['tx_n']
    new_recipient = tx_data['new_recipient']

    if blk_n < 0 or blk_n > len(blockchain.chain) - 1:
        return "Invalid data...", 400

    blockchain.chain[blk_n].transactions[tx_n]['Recipient'] = new_recipient

    return "Changed the chain!", 200

@app.route('/new_transaction', methods = ['POST'])
def new_transaction():
    tx_data = request.get_json()
    required_fields = ["Recipient", "Sender", "Amount"]

    for field in required_fields:
        if not tx_data.get(field):
            return "Invalid transaction data", 400

    tx_data["timestamp"] = time.time()

    blockchain.add_new_transaction(tx_data)

    if blockchain.unconfirmed_tx == Blockchain.max_unconfirmed_txs:
        requests.get('http://127.0.0.1:{}/mine'.format(port))
        return "Automatically mined block #{} as we reached {} pending transactions".format(blockchain.last_block.index, Blockchain.max_unconfirmed_txs), 201

    return "Success", 201

###################################################################################################################################################
###################################################################################################################################################
###################################################################################################################################################

def shutdown_server():
    func = request.environ.get('werkzeug.server.shutdown')
    if func is None:
        raise RuntimeError('Not running with the Werkzeug Server')
    func()

@app.route('/shutdown', methods = ['POST'])
def shutdown():
    shutdown_server()
    return 'Server shutting down...'

@app.route('/chain', methods = ['GET'])
def get_chain():
    chain_data = []
    for block in blockchain.chain:
        chain_data.append(block.__dict__)
    
    response = {
        "length": len(chain_data),
        "chain": chain_data,
        "peers": list(peers)
    }
    return jsonify(response), 200

@app.route('/mine', methods = ['GET'])
def mine_unconfirmed_transactions():
    result = blockchain.mine()
    if not result:
        return "No transactions to mine"
    else:
        # Conseguimos la longitud de nuestro blockchain
        chain_length = len(blockchain.chain)
        consensus()
        if chain_length == len(blockchain.chain):
            # Si nuestro blockchain era el más largo anunciamos que hemos añadido un bloque nuevo a los demás.
            announce_new_block(blockchain.last_block)
        return "Block #{} is mined.".format(blockchain.last_block.index)

@app.route('/register_node', methods = ['POST'])
def register_new_peers():
    node_address = request.get_json()["node_address"]
    if not node_address:
        return "Invalid data", 400

    # Añadimos el nodo a la lista
    peers.add(node_address)

    # Le pasamos el blockchain actual para que se sincronice con nosotros
    return get_chain()

@app.route('/register_with', methods = ['POST'])
def register_with_existing_node():
    node_address = request.get_json()["node_address"]
    if not node_address:
        return "Invalid data", 400

    data = {"node_address": request.host_url}
    headers = {'Content-Type': "application/json"}

    # Hacemos una petición para registrarnos con el nodo que especifiquemos al hacer la petición a '/register_with'
    response = requests.post(node_address + "/register_node", data = json.dumps(data), headers = headers)

    # Si nos hemos registrado correctamente
    if response.status_code == 200:
        global blockchain
        global peers
        
        # Actualizamos nuestra copia del blockchain y la lista de peers o nodos de la red
        chain_dump = response.json()['chain']
        blockchain = create_chain_from_dump(chain_dump)
        peers.update(response.json()['peers'])

        # Devolvemoos un 200 OK
        return "Registration successful", 200
    else:
        # Si ocurre algún error lo maneja la API de response
        return response.content, response.status_code

def create_chain_from_dump(chain_dump):
    generated_blockchain = Blockchain()
    for idx, block_data in enumerate(chain_dump):
        if idx == 0:
            # Nos saltamos el bloque génesis ya que ya lo hemos creado al instanciar un bloockchain vacío.
            continue
        block = Block(block_data["index"],
                      block_data["transactions"],
                      block_data["timestamp"],
                      block_data["previous_hash"],
                      block_data["nonce"])
        proof = block_data['hash']

        # La función add_block() comprueba que el bloque sea válido antes de añadirlo. Si no se ha añadido un
        # bloque lanzamos una excepción avisando de que la cadena que hemos recibido no es correcta y ha sido
        # modificada.
        added = generated_blockchain.add_block(block, proof)
        if not added:
            raise Exception("The chain dump is tampered!!")

    # Si todo ha ido bien se devuelve la cadena construida.
    return generated_blockchain

@app.route('/add_block', methods = ['POST'])
def verify_and_add_block():
    block_data = request.get_json()
    block = Block(block_data["index"],
                  block_data["transactions"],
                  block_data["timestamp"],
                  block_data["previous_hash"],
                  block_data["nonce"])

    proof = block_data['hash']
    added = blockchain.add_block(block, proof)

    if not added:
        return "The block was discarded by the node", 400

    return "Block added to the chain", 201


# Al hacer peticiones a '/pending_tx' se devuelve la lista de transacciones pendientes del nodo como
# un objeto JSON

@app.route('/pending_tx')
def get_pending_tx():
    return json.dumps(blockchain.unconfirmed_transactions)

def consensus():
    global blockchain

    longest_chain = None
    current_len = len(blockchain.chain)

    for node in peers:
        # Conseguimos un objeto que contiene la longitud de la cadena a través del
        # endpoint '/chain' definido en la línea 387
        response = requests.get('{}chain'.format(node))

        # Accedemos a elementos de este objeto
        length = response.json()['length']
        chain = response.json()['chain']
        if length > current_len and blockchain.check_chain_validity(chain):
            current_len = length
            longest_chain = chain

    if longest_chain:
        blockchain = longest_chain
        return True

    return False

def announce_new_block(block):
    for peer in peers:
        url = "{}add_block".format(peer)
        headers = {'Content-Type': "application/json"}
        requests.post(url, data = json.dumps(block.__dict__, sort_keys = True), headers = headers)

if __name__ == '__main__':
    from argparse import ArgumentParser

    parser = ArgumentParser()
    parser.add_argument('-p', '--port', default = 8000, type = int, help = 'port to listen on')
    args = parser.parse_args()
    port = args.port

    app.run(host = '0.0.0.0', port = port)