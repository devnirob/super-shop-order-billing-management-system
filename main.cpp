#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
using namespace std;

const int MAX_PRODUCTS = 50;
const int MAX_CART = 30;
const int LOW_STOCK = 5;

struct Product {
    int id;
    string name;
    double price;
    int stock;
};

struct CartItem {
    int productIndex;
    int quantity;
};

struct OrderItem {
    string name;
    int quantity;
    double price;
};

Product products[MAX_PRODUCTS];
CartItem cart[MAX_CART];
int productCount = 0;
int cartCount = 0;

int askInt(const string& prompt, int minimum, int maximum) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

bool saveProducts() {
    ofstream file("products.txt");
    if (!file) return false;
    file << fixed << setprecision(2);
    for (int i = 0; i < productCount; i++) {
        file << products[i].id << ' ' << products[i].name << ' '
             << products[i].price << ' ' << products[i].stock << '\n';
    }
    return bool(file);
}

void loadProducts() {
    ifstream file("products.txt");
    while (productCount < MAX_PRODUCTS &&
           file >> products[productCount].id >> products[productCount].name
                >> products[productCount].price >> products[productCount].stock) {
        productCount++;
    }
    if (productCount == 0) {
        products[0] = {1, "Milk", 3.50, 8};
        products[1] = {2, "Bread", 2.00, 10};
        products[2] = {3, "Rice", 5.00, 12};
        products[3] = {4, "Eggs", 4.00, 8};
        products[4] = {5, "Soap", 1.50, 15};
        productCount = 5;
        if (!saveProducts()) cout << "Could not save products.\n";
    }
}

int productIndex(int id) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) return i;
    }
    return -1;
}

int cartIndex(int product) {
    for (int i = 0; i < cartCount; i++) {
        if (cart[i].productIndex == product) return i;
    }
    return -1;
}

void lowStockAlerts() {
    for (int i = 0; i < productCount; i++) {
        if (products[i].stock < LOW_STOCK) {
            cout << "Low stock: " << products[i].name << " ("
                 << products[i].stock << " left)\n";
        }
    }
}

void showProducts() {
    cout << "\nID  Product       Price  Stock\n";
    for (int i = 0; i < productCount; i++) {
        cout << setw(2) << products[i].id << "  "
             << left << setw(12) << products[i].name << right
             << setw(5) << products[i].price << "  " << products[i].stock << '\n';
    }
    lowStockAlerts();
}

void showCart() {
    if (cartCount == 0) {
        cout << "Order is empty.\n";
        return;
    }
    double subtotal = 0;
    cout << "\nCurrent order:\n";
    for (int i = 0; i < cartCount; i++) {
        Product& p = products[cart[i].productIndex];
        double lineTotal = p.price * cart[i].quantity;
        cout << p.id << ' ' << p.name << " x " << cart[i].quantity
             << " = " << lineTotal << '\n';
        subtotal += lineTotal;
    }
    cout << "Subtotal: " << subtotal << '\n';
}

void addItem() {
    showProducts();
    int id = askInt("Product ID (0 to cancel): ", 0, 9999);
    if (id == 0) return;
    int p = productIndex(id);
    if (p < 0) { cout << "Product not found.\n"; return; }
    if (products[p].stock == 0) { cout << "Out of stock.\n"; return; }
    int quantity = askInt("Quantity: ", 1, products[p].stock);
    int c = cartIndex(p);
    if (c >= 0) {
        if (cart[c].quantity + quantity > products[p].stock) {
            cout << "Not enough stock.\n";
            return;
        }
        cart[c].quantity += quantity;
    } else {
        if (cartCount == MAX_CART) { cout << "Order is full.\n"; return; }
        cart[cartCount++] = {p, quantity};
    }
    cout << "Added to order.\n";
}

void removeAt(int position) {
    for (int i = position; i < cartCount - 1; i++) cart[i] = cart[i + 1];
    cartCount--;
}

void changeItem() {
    showCart();
    if (cartCount == 0) return;
    int id = askInt("Product ID to change: ", 1, 9999);
    int c = cartIndex(productIndex(id));
    if (c < 0) { cout << "Product is not in the order.\n"; return; }
    int quantity = askInt("New quantity (0 removes it): ", 0,
                          products[cart[c].productIndex].stock);
    if (quantity == 0) removeAt(c);
    else cart[c].quantity = quantity;
    cout << "Order updated.\n";
}

void removeItem() {
    showCart();
    if (cartCount == 0) return;
    int id = askInt("Product ID to remove: ", 1, 9999);
    int c = cartIndex(productIndex(id));
    if (c < 0) { cout << "Product is not in the order.\n"; return; }
    removeAt(c);
    cout << "Product removed.\n";
}

int nextOrderId() {
    ifstream file("orders.txt");
    string line, word;
    int id, next = 1;
    while (getline(file, line)) {
        istringstream input(line);
        if (input >> word >> id && word == "ORDER" && id >= next) next = id + 1;
    }
    return next;
}

double billTotal(double subtotal, int discount) {
    return round(subtotal * (100 - discount)) / 100;
}

void printReceipt(ostream& out, int id, OrderItem items[], int count,
                  double subtotal, int discount) {
    out << fixed << setprecision(2);
    out << "\nOrder #" << id << '\n';
    for (int i = 0; i < count; i++) {
        out << items[i].name << " x " << items[i].quantity << " @ "
            << items[i].price << " = " << items[i].price * items[i].quantity << '\n';
    }
    out << "Subtotal: " << subtotal << '\n';
    out << "Discount: " << discount << "%\n";
    out << "Total: " << billTotal(subtotal, discount) << '\n';
}

void checkout() {
    if (cartCount == 0) { cout << "Order is empty.\n"; return; }
    showCart();
    int discount = askInt("Discount percent (0-100): ", 0, 100);
    int id = nextOrderId();
    double subtotal = 0;
    OrderItem items[MAX_CART];
    for (int i = 0; i < cartCount; i++) {
        Product& p = products[cart[i].productIndex];
        items[i] = {p.name, cart[i].quantity, p.price};
        subtotal += p.price * cart[i].quantity;
    }

    ofstream file("orders.txt", ios::app);
    if (!file) { cout << "Could not save order.\n"; return; }
    file << fixed << setprecision(2);
    file << "ORDER " << id << ' ' << subtotal << ' ' << discount << ' '
         << cartCount << '\n';
    for (int i = 0; i < cartCount; i++) {
        file << items[i].name << ' ' << items[i].quantity << ' '
             << items[i].price << '\n';
    }
    file.close();
    if (!file) { cout << "Could not save order.\n"; return; }

    int itemCount = cartCount;
    for (int i = 0; i < cartCount; i++) {
        products[cart[i].productIndex].stock -= cart[i].quantity;
    }
    cartCount = 0;
    if (!saveProducts()) cout << "Warning: stock could not be saved.\n";
    printReceipt(cout, id, items, itemCount, subtotal, discount);
    ofstream receipt("receipt_" + to_string(id) + ".txt");
    if (receipt) printReceipt(receipt, id, items, itemCount, subtotal, discount);
    else cout << "Could not save receipt.\n";
    lowStockAlerts();
}

void searchOrder() {
    int wanted = askInt("Order ID: ", 1, 999999);
    ifstream file("orders.txt");
    string line, word;
    while (getline(file, line)) {
        istringstream header(line);
        int id, discount, count;
        double subtotal;
        if (!(header >> word >> id >> subtotal >> discount >> count) ||
            word != "ORDER") continue;
        if (count < 0 || count > MAX_CART) break;
        OrderItem items[MAX_CART];
        for (int i = 0; i < count; i++) {
            if (!(file >> items[i].name >> items[i].quantity >> items[i].price)) return;
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (id == wanted) {
            printReceipt(cout, id, items, count, subtotal, discount);
            return;
        }
    }
    cout << "Order not found.\n";
}

void restock() {
    showProducts();
    int id = askInt("Product ID to restock: ", 1, 9999);
    int p = productIndex(id);
    if (p < 0) { cout << "Product not found.\n"; return; }
    int quantity = askInt("Units to add: ", 1, 100000);
    products[p].stock += quantity;
    if (saveProducts()) cout << "Stock updated.\n";
    else cout << "Could not save stock.\n";
}

void salesReport() {
    ifstream file("orders.txt");
    string line, word;
    int count = 0;
    double total = 0;
    while (getline(file, line)) {
        istringstream header(line);
        int id, discount, itemCount;
        double subtotal;
        if (header >> word >> id >> subtotal >> discount >> itemCount &&
            word == "ORDER") {
            count++;
            total += billTotal(subtotal, discount);
        }
    }
    cout << "Orders: " << count << "\nSales total: " << total << '\n';
}

int main() {
    cout << fixed << setprecision(2);
    loadProducts();
    lowStockAlerts();
    while (true) {
        cout << "\nSUPER SHOP\n"
             << "1. Products  \n2. Add to order  \n3. Change quantity\n"
             << "4. Remove item  \n5. View order  \n6. Checkout\n"
             << "7. Search order  \n8. Restock  \n9. Sales report  \n0. Exit\n\n";
        int choice = askInt("Choice: ", 0, 9);
        if (choice == 0) break;
        if (choice == 1) showProducts();
        else if (choice == 2) addItem();
        else if (choice == 3) changeItem();
        else if (choice == 4) removeItem();
        else if (choice == 5) showCart();
        else if (choice == 6) checkout();
        else if (choice == 7) searchOrder();
        else if (choice == 8) restock();
        else if (choice == 9) salesReport();
    }
}
