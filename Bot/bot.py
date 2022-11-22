import discord
from discord.ext import commands

import matplotlib.pyplot as plt
import numpy as np

token = ""

bot = commands.Bot(intents = discord.Intents.default(),
                   command_prefix = "!")

def getImage(x, y):
    x = np.array(x)
    y = np.array(y)

    plt.plot(x, y, 'r')

    plt.grid(True)
    plt.savefig("plot.png")

@bot.command()
async def plot(Content):
    print(Content.message.author, Content.message.content)

    function = Content.message.content.split()[1:]

    try:
        await Content.send(file = discord.File("ov4.jpg"))
    except:
        await Content.send("ERROR: Couldn't parse your func!")

bot.run(token)
