#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
using namespace std;

class Product
{
private:
	string productName;
	//int productQuantity;
	double productPrice;

public:
	int productQuantity;
	explicit Product() = default;
	explicit Product (string _productName, int _productQuantity, double _productPrice) :
		productName (_productName),
		productQuantity (_productQuantity),
		productPrice(_productPrice){}

	[[nodiscard]] string getProductName() const noexcept { return productName; }
	[[nodiscard]] int getProductQuantity() const noexcept { return productQuantity; }
	[[nodiscard]] double getProductPrice() const noexcept { return productPrice; }

	void updateQuantity(int quantity)
	{
		productQuantity = quantity;
	}

	//Serialization
	friend ostream& operator<< (ostream& os, const Product& product)
	{
		return os << product.productName << ' '
			<< product.productPrice << ' '
			<< product.productQuantity;
	}

	friend istream& operator >> (istream& is, Product& product)
	{
		return is >> product.productName
			>> product.productPrice
			>> product.productQuantity;
	}

};


class DataBase
{
private:
	vector <Product> products;
	vector <Product> cart;
	string storeName;

public:
	explicit DataBase() = default;
	explicit DataBase (string& _storeName) : storeName (_storeName){}

	[[nodiscard]] const auto& getAllProducts() const noexcept { return products; }
	[[nodiscard]] const auto& getAllCart() const noexcept { return cart; }
	void addProduct (const Product& product) { products.push_back(product); }
	void clearCart() { cart.clear(); }

	[[nodiscard]] auto& getProduct(string productName)
	{
		 auto it = find_if(products.begin(), products.end(), [productName](const Product& product)
			{return product.getProductName() == productName; 
			});

		if (it != products.end()) { return *it; }
		else
		{
			throw runtime_error("Product Not Found!\n");
		}
		
	}

	void updateQuantity(string productName, int productQuantity)
	{
		auto it = find_if(products.begin(), products.end(), [productName] (const Product& product)
		{
			return product.getProductName() == productName;
		});
		if (it != products.end()) { it->productQuantity = productQuantity; }
	}
	
	/*
	void updateQuantity(string productName, int productQuantity)
	{
		try
		{
			auto& product = getProduct(productName);
			//product.updateQuantity(productQuantity);
		}
		catch (const exception& e) {
			cout << e.what();
		}	
	}
	*/

	void addToCart(const Product& product) { cart.push_back(product); }
	

	void saveToFile() const
	{
		ofstream file(storeName, ios::binary);
		if (!file)
			throw runtime_error("File fail to open\n");
		for (const auto& product : products)
		{
			file << product << '\n';
		}
		file.close();
	}

	void loadFromFile()
	{
		ifstream file(storeName, ios::binary);
		if (!file)
			throw runtime_error("File fail to open\n");

		products.clear();
		string line;
		while (getline(file, line))
		{
			Product product;
			istringstream iss(line);
			if (!(iss >> product)) {
				throw runtime_error("Fail to read client");
			}

			products.push_back(product);
		}

	}
};

inline int logIn()
{
	int action;
	system("cls");
	cout << "\n\t\tLog IN as:\n\n"
		<< "\t1 -> Admin\n"
		<< "\t2 -> Sales Rep\n"
		<< "\t3 -> Exit\n";
	cout << "Action: ";
	cin >> action;
	return action;
}

inline int promptAdminLogin()
{
	int action;
	system("cls");
	cout << "\n\t\tAdmin Login\n"
		<< "\tEnter Password: ";
	cin >> action;
	return action;
}
inline int promptSalesRepLogin()
{
	int action;
	system("cls");
	cout << "\n\t\tSales Rep Login\n"
		<< "\tEnter Password: ";
	cin >> action;
	return action;
}
inline int promptAdmin()
{
	int action;
	system("cls");
	cout << "\n\t\tAdmin Loged In\n"
		<< "\t1 -> Add Product\n"
		<< "\t2 -> Check Product Details\n"
		<< "\t3 -> Sell Product\n"
		<< "\t4 -> Check Out\n"
		<< "\t5 -> Exit\n";
	cout << "Action: ";
	cin >> action;
	return action;
}
inline int promptSalesRep()
{
	int action;
	system("cls");
	cout << "\n\t\tSales Rep\n"
		<< "\t1 -> Sale Product\n"
		<< "\t2 -> Check out\n"
		<< "\t3 -> Exit\n";
	cout << "Action: ";
	cin >> action;
	return action;
}

/*
inline void sellProduct (DataBase& database)
{
	string productName;
	double productPrice;
	int productQuantity, productQuantityy;
	cout << "Enter Product Name: ";
	cin >> productName;
	
	try
	{
		auto& product = database.getProduct(productName);
		cout << "Enter Quantity: ";
		cin >> productQuantity;
		if (productQuantity > product.getProductQuantity())
		{
			cout << "Product Out of stock.";
			system("pause");
		}
		else
		{
			productQuantityy = product.getProductQuantity();
			productQuantityy -= productQuantity;
			
			product.updateQuantity(productQuantityy);
			productPrice = product.getProductPrice();
			database.addToCart(Product(productName, productQuantity, productPrice));
			cout << "Product Added to cart.";
			system("pause");
		}

	}
	catch (const exception& e) {
		cout << e.what();
		system("pause");
	}
}

inline void checkOut(DataBase& database)
{
	double total;
	
	auto& cart = database.getAllCart();
	if (cart.empty())
	{
		cout << "No goods in cart.";
		system("pause");
	}
	else
	{
		system("cls");
		cout << "\n\t\tList of Products to pay for:\n"
			<< "Name"
			<< "\tQuantity"
			<< "\tPrice\n";
		for (auto& product : cart)
		{
			cout << product.getProductName()
				<< "\t" << product.getProductQuantity()
				<< "\t" << product.getProductPrice() << "\n";
			total = total + (product.getProductQuantity() * product.getProductPrice());
		}
		cout << "Total = " << total << endl;
		database.clearCart();
		system("pause");
	}
}
*/
int main()
{
	string storeName = "data.txt";
	DataBase database(storeName);
	string productName;
	int productQuantity;
	double productPrice;
	try
	{
		database.loadFromFile();
	}catch (const exception& e)
	{
		cout << e.what();
	}

	int actionLogin;

	do
	{
		actionLogin = logIn();
		switch (actionLogin)
		{
		case 1:
		{
			system("cls");
			int actionAdminLogin;
			do
			{
				actionAdminLogin = promptAdminLogin();
			} while (actionAdminLogin != 2023);

			int actionAdmin;
			do
			{
				actionAdmin = promptAdmin();
				switch (actionAdmin)
				{
				case 1:
				{
					
					system("cls");
					cout << "\n\t\tAdd Product:\n";
					cout << "\tEnter Product Name: ";
					cin >> productName;
					try
					{
						auto& product = database.getProduct(productName);
						cout << "\nProduct already in stock.";
						system("pause");
						break;
					}catch (const exception& e){}

					cout << "\n\tEnter product price: ";
					cin >> productPrice;
					cout << "\n\tEnter available quantity: ";
					cin >> productQuantity;
					database.addProduct(Product(productName, productQuantity, productPrice));
					database.saveToFile();
					cout << "\nproduct Saved.";
					system("pause");
					break;
				}
				case 2:
				{
					const auto& products = database.getAllProducts();
					if (products.empty())
					{
						cout << "\n\tNo product found.";
					}
					else
					{
						system("cls");
						cout << "\n\t\tList of Products\n"
							<< "Name"
							<< "\tQuantity"
							<< "\tPrice\n";
						for (const auto& product : products)
						{
							cout << product.getProductName()
								<< "\t" << product.getProductQuantity()
								<< "\t\t" << product.getProductPrice() << "\n";
						}
					}
					system("pause");
					break;
				}
				case 3:
				{
					//sellProduct(database);
					
					cout << "Enter Product Name: ";
					cin >> productName;
					try
					{
						auto& product = database.getProduct(productName);
						cout << "Enter Product Quantity: ";
						cin >> productQuantity;
						int productQuantityNow;
						productQuantityNow = product.getProductQuantity();
						//cout << "Ok ";
						if (productQuantityNow > productQuantity)
						{
							
							productQuantityNow -= productQuantity;
							//database.updateQuantity(productName, productQuantityNow);
							product.updateQuantity(productQuantityNow);
							productPrice = product.getProductPrice();
							database.addToCart(Product(productName, productQuantity, productPrice));
							cout << "Product Added to cart.";
							system("pause");
						}
						else
						{
							cout << "Product out of Stock " << productQuantity << " " << productQuantityNow - 2;
							system("pause");
						}
					}
					catch (const exception& e)
					{
						cout << e.what();
						system("pause");
					}
					break;
				}
				case 4:
				{
					//checkOut(database);
					double total = 0.0;

					auto& cart = database.getAllCart();
					if (cart.empty())
					{
						cout << "No goods in cart.";
						system("pause");
					}
					else
					{
						system("cls");
						cout << "\n\t\tList of Products to pay for:\n"
							<< "Name"
							<< "\tQuantity"
							<< "\tPrice\n";
						for (auto& product : cart)
						{
							cout << product.getProductName()
								<< "\t" << product.getProductQuantity()
								<< "\t\t" << product.getProductPrice() << "\n";
							//productQuantity = product.getProductQuantity();
							//productPrice = product.getProductPrice();
							//total = total + (productQuantity * productPrice);
							total = total + (product.getProductQuantity() * product.getProductPrice());
						}
						cout << "Total = " << total << endl;
						database.clearCart();
						database.saveToFile();
						system("pause");
					}
					break;
				}
				case 5:
				{
					cout << "\nExiting-------------\n";
					system("pause");
					break;
				}
				default:
				{
					cout << "\nInvalid Input. Try again.";
					system("pause");
					break;
				}
				}
			} while (actionAdmin != 5);
			break;
		}
		case 2:
		{
			int actionSalesRepLogin;

			do
			{
				actionSalesRepLogin = promptSalesRepLogin();

				
			} while (actionSalesRepLogin != 1234);

			int actionSalesRep;

			do
			{
				actionSalesRep = promptSalesRep();
				switch (actionSalesRep)
				{
				case 1:
				{
					cout << "Enter Product Name: ";
					cin >> productName;
					try
					{
						auto& product = database.getProduct(productName);
						cout << "Enter Product Quantity: ";
						cin >> productQuantity;
						int productQuantityNow;
						productQuantityNow = product.getProductQuantity();
						//cout << "Ok ";
						if (productQuantityNow > productQuantity)
						{

							productQuantityNow -= productQuantity;
							//database.updateQuantity(productName, productQuantityNow);
							product.updateQuantity(productQuantityNow);
							database.saveToFile();
							productPrice = product.getProductPrice();
							database.addToCart(Product(productName, productQuantity, productPrice));
							cout << "\nProduct Added to cart.";
							system("pause");
						}
						else
						{
							cout << "Product out of Stock " << productQuantity << " " << productQuantityNow - 2;
							system("pause");
						}
					}
					catch (const exception& e)
					{
						cout << e.what();
						system("pause");
					}
					break;
				}
				case 2:
				{
					double total = 0.0;

					auto& cart = database.getAllCart();
					if (cart.empty())
					{
						cout << "No goods in cart.";
						system("pause");
					}
					else
					{
						system("cls");
						cout << "\n\t\tList of Products to pay for:\n"
							<< "Name"
							<< "\tQuantity"
							<< "\tPrice\n";
						for (auto& product : cart)
						{
							cout << product.getProductName()
								<< "\t" << product.getProductQuantity()
								<< "\t\t" << product.getProductPrice() << "\n";
							//productQuantity = product.getProductQuantity();
							//productPrice = product.getProductPrice();
							//total = total + (productQuantity * productPrice);
							total = total + (product.getProductQuantity() * product.getProductPrice());
						}
						cout << "Total = " << total << endl;
						database.clearCart();
						database.saveToFile();
						system("pause");
					}
					break;
				}
				case 3:
				{
					cout << "\nExiting-------------\n";
					system("pause");
					break;
				}
				default:
				{
					cout << "\nInvalid Input. Try again.";
					system("pause");
					break;
				}
				}
			} while (actionSalesRep != 3);

			break;
		}
		case 3:
		{
			cout << "\nExiting the program ==================!!!\n";
			system("pause");
			break;
		}
		default:
		{
			cout << "\nInvalid Input!--------------------!";
			system("pause");
			break;
		}
		}
	} while (actionLogin != 3);



	return 0;
}