<?php

class CgttBaseMigration extends Migration {
	protected $cgttConection = 'default';
	protected $cgttConectionKey = 'migrate_cgtt';



	/**
	 * Return the default database conection
	 * migrated.
	 */
	public function getConection() {
		return $this->cgttConection;

	}

	/**
	 * Return the remote database conection
	 * migrated.
	 */
	public function getConectionKey() {
		return $this->cgttConectionKey;
	}


	/**
	 * Return the table name to be
	 * migrated.
	 */
	public function __construct($arguments) {
		parent::__construct($arguments);
		$this->team = array(
			new MigrateTeamMember('Leydis Contreras', 'ldcontreras@emergya.com',
				t('Developer')),
		);


	}







}