class Item:
    def __init__(self, name, weight):
        self.name = name  # Name of the item
        self.weight = weight  # Weight of the item in kilograms

    def output_item(self):
        print(f"{self.name}, {self.weight} kg")  # Output the item's name and weight


# Create three items
item1 = Item("Laptop", 2)
item2 = Item("Book", 1)
item3 = Item("Dumbbell", 4)

# Output the items
item1.output_item()
item2.output_item()
item3.output_item()
