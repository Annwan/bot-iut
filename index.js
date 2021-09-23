const { REST } = require('@discordjs/rest');
const { SlashCommandBuilder, SlashCommandStringOption } = require('@discordjs/builders');
const { Routes } = require('discord-api-types/v9');
const { token, clientid, guildid } = require('./config.json');
const { Client, Intents } = require('discord.js');
const edt = require('./modules/edt.js')

const commands = [
    new SlashCommandBuilder()
        .setName('ping')
        .setDescription('Replies with pong'),
    new SlashCommandBuilder()
        .setName('edt')
        .setDescription('Gets a time table')
        .addStringOption(
            new SlashCommandStringOption()
                .setName('group')
                .setDescription('The group')
                .setRequired(false)
        )
        .addStringOption(
            new SlashCommandStringOption()
                .setName('date')
                .setDescription('The date')
                .setRequired(false)
        )
]
      .map(command => command.toJSON());

const rest = new REST({ version: '9' }).setToken(token);
const client = new Client({intents : [Intents.FLAGS.GUILDS]});

(async () => {
    try {
        await rest.put(
            Routes.applicationGuildCommands(clientid, guildid),
            {body: commands}
        );
        console.log("Commands successfully reloaded");
        console.log(commands);
    } catch (error) {
        console.error(error);
    }
})();

client.once('ready', () => {})

client.on('interactionCreate', async interaction => {
    if (!interaction.isCommand()) return;

    const { commandName } = interaction;

    switch (commandName) {
        case 'ping':
            await interaction.reply('Pong');
            break;
        case 'edt':
            await edt(interaction);
            break
        default:
            await interaction.reply(`[Not Implemented]: ${commandName}`);
    }


});

client.login(token);
