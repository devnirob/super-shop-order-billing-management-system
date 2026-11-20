# Super Shop Order and Billing Management System

A simple console application written in **C++** for managing supermarket orders, billing and stock. Created as a university Software Development Lab project with arrays, structures, loops, functions and file handling.

## Features

- Display products with their IDs, prices and stock quantities.
- Add multiple products to a customer order.
- Change item quantities or remove items before checkout.
- View the current order and subtotal.
- Apply a whole-number discount from 0% to 100%.
- Generate a bill and save a printable text receipt.
- Search previous orders by order ID.
- Restock products by adding more units.
- Display total completed orders and total sales after discounts.
- Warn when a product has fewer than 5 units remaining.
- Save products and completed orders in local text files.

## Requirements

A C++ compiler supporting **C++11 or later**, such as GCC/G++. No external libraries or database are required. You can use VS Code, Code::Blocks or a terminal.

## Build and run

Download this repository, then open a terminal in its folder.

### Linux / macOS

```bash
g++ -std=c++11 -Wall -Wextra main.cpp -o super_shop
./super_shop
```

### Windows (MinGW/G++)

```powershell
g++ -std=c++11 -Wall -Wextra main.cpp -o super_shop.exe
.\super_shop.exe
```

In Code::Blocks, create a console C++ project, use this `main.cpp`, enable C++11 or newer, and choose **Build and Run**.

## Menu

| Option | Action |
| --- | --- |
| 1 | Display products |
| 2 | Add a product to the current order |
| 3 | Change quantity; enter 0 to remove the item |
| 4 | Remove a product from the current order |
| 5 | View the current order |
| 6 | Checkout, apply discount and save the bill |
| 7 | Search a completed order by ID |
| 8 | Add stock to a product |
| 9 | View the sales report |
| 0 | Exit |

## Example demonstration

1. Choose **1** to view the sample products.
2. Choose **2**, enter product ID **1** (Milk), then quantity **2**.
3. Choose **2**, enter product ID **2** (Bread), then quantity **1**.
4. Choose **5**: the subtotal is **9.00**.
5. Choose **6** and enter a **10** percent discount: the total is **8.10**.
6. Note the order ID printed on the bill.
7. Choose **7** and enter that ID to view the saved order.
8. Choose **9** to view the sales summary.

Checkout reduces stock. Changing or removing items before checkout does not reduce stock. An unfinished order is discarded when you exit.

## Files

| File | Purpose |
| --- | --- |
| `main.cpp` | Complete C++ source code |
| `README.md` | Project explanation and instructions |
| `.gitignore` | Excludes build output and runtime sales files |
| `products.txt` | Automatically created product inventory |
| `orders.txt` | Completed orders, appended at checkout |
| `receipt_<ID>.txt` | Printable receipt for a completed order |

On the first run, the program creates five sample products: Milk, Bread, Rice, Eggs and Soap. These are demonstration prices; the program does not enforce a currency. Runtime files are created in the current working directory. Run it from the same folder to keep using the same records, and keep backups.

### Data format

`products.txt` stores one product per line:

```text
1 Milk 3.50 8
```

The fields are product ID, single-word name, unit price and available stock.

`orders.txt` stores an order header followed by its item lines:

```text
ORDER 1 9.00 10 2
Milk 2 3.50
Bread 1 2.00
```

The header contains the order ID, subtotal, discount percentage and number of item lines. Item lines store the name, quantity and price at checkout so old bills retain their original prices.

## How the code works

| Structure | Stores |
| --- | --- |
| `Product` | ID, name, price and stock |
| `CartItem` | Product array position and ordered quantity |
| `OrderItem` | Product name, quantity and price saved with a bill |

Global arrays store products and the active cart. `main()` repeats the menu and calls a small function for each action. `askInt()` handles numeric input, `checkout()` saves the order and updates stock, `printReceipt()` formats the bill, and `searchOrder()` reads saved bills.

The bill calculation is:

```text
Total = Subtotal × (100 − Discount Percentage) / 100
```

The result is rounded to two decimal places.

## Easy changes for practice

- Change `LOW_STOCK` to adjust the warning threshold.
- Change `MAX_PRODUCTS` or `MAX_CART` to adjust array capacities.
- Edit the sample products in `loadProducts()` before the first run. Existing saved products take precedence.
- Edit `printReceipt()` to change the bill layout.
- Edit `billTotal()` to change the discount calculation.

## Current limitations

- Console interface; no login, GUI, database, tax, refund or payment integration.
- Maximum 50 products and 30 different products in an active order.
- Product names in saved files must be a single word; use an underscore for multiword names.
- Product creation and price editing are not provided as menu actions.
- Only whole-number discounts are supported.
- Run one instance at a time; simultaneous use is not supported.
- Inventory and orders use separate files. If an inventory save fails after an order is recorded, stock and bills can become inconsistent. This is an educational project rather than a production shop system.
- Malformed or manually damaged data files are not fully handled.

## Academic use

Understand the functions and practice adding, editing, removing, checking out and searching orders before the demonstration. The repository begins with this upload; continue making meaningful commits as you develop the project.
